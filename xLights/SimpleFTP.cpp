#include "SimpleFTP.h"
#include <log4cpp/Category.hh>
#include <wx/progdlg.h>
#include <wx/msgdlg.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/log.h>

SimpleFTP::SimpleFTP(std::string ip, std::string user, std::string password)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    ftp.SetUser(user);
    ftp.SetPassword(password);
    if (!ftp.Connect(ip))
    {
        logger_base.warn("Could not connect using address '%s'.", (const char *)ip.c_str());
        wxMessageBox("Could not connect using address '%s'.", (const char *)ip.c_str());
    }
}

bool SimpleFTP::IsConnected()
{
    return (ftp.IsConnected() && ftp.IsOk() && !ftp.IsClosed());
}

bool SimpleFTP::UploadFile(std::string file, std::string folder, std::string newfilename, bool backup, bool binary, wxWindow* parent)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    if (!IsConnected()) return false;

    ftp.ChDir(folder);

    if (binary)
    {
        ftp.SetBinary();
    }
    else
    {
        ftp.SetAscii();
    }

    wxProgressDialog progress("FTP Upload", wxString(file.c_str()), 100, parent, wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_AUTO_HIDE);
    progress.Show();

    bool cancelled = false;

    progress.Update(0, wxEmptyString, &cancelled);

    wxFileName fn(wxString(file.c_str()));
    wxString ext = fn.GetExt();
    if (ext != "") ext = "." + ext;
    wxString basefile = fn.GetName() + ext;

    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets

    //logger_base.info("FTP current directory %s.", (const char *)ftp.Pwd().c_str());

    if (backup)
    {
        int size = ftp.GetFileSize((folder + "/" + basefile).c_str());
        if (size == -1)
        {
            // file not there so no need to backup
        }
        else
        {
            wxDateTime dt = wxDateTime::Now();
            wxString tgtfile = wxString((folder + "/" + basefile).c_str()) + "_" + dt.Format("%Y%m%d_%H%M%S");
            logger_base.info("FTP Backing up file %s to %s.", (const char *)(folder + "/" + basefile).c_str(), (const char *)tgtfile.c_str());
            ftp.Rename((folder + "/" + basefile).c_str(), tgtfile);
            if (!cancelled)
            {
                cancelled = progress.WasCancelled();
            }
        }
    }

    wxFile in;
    in.Open(wxString(file.c_str()));
    if (in.IsOpened())
    {
        logger_base.info("FTP Uploading file %s to %s.", (const char *)file.c_str(), (const char *)(folder + "/" + basefile).c_str());
        wxFileOffset length = in.Length();
        wxFileOffset done = 0;
        wxOutputStream *out = ftp.GetOutputStream((folder + "/" + basefile).c_str());
        if (out)
        {
            uint8_t buffer[8192]; // 8KB at a time
            while (!in.Eof() && !cancelled)
            {
                ssize_t read = in.Read(&buffer[0], sizeof(buffer));
                out->WriteAll(&buffer[0], read);
                done += read;
                progress.Update((done * 100) / length, wxEmptyString, &cancelled);
                if (!cancelled)
                {
                    cancelled = progress.WasCancelled();
                }
            }
            if (in.Eof())
            {
                progress.Update(100, wxEmptyString, &cancelled);
                logger_base.info("   FTP Upload of file %s done.", (const char *)file.c_str());
            }
            else
            {
                progress.Update(100, wxEmptyString, &cancelled);
                logger_base.warn("   FTP Upload of file %s cancelled.", (const char *)file.c_str());
            }
            in.Close();
            out->Close();
            delete out;
            if (ftp.GetFileSize((folder + "/" + basefile).c_str()) != length)
            {
                logger_base.warn("   FTP Upload of file %s failed. Source size (%d) != Destination Size (%d)", (const char *)file.c_str(), length, ftp.GetFileSize((folder + "/" + basefile).c_str()));
            }
        }
        else
        {
            wxMessageBox("FTP Upload of file failed to create the target file.");
            progress.Update(100, wxEmptyString, &cancelled);
            logger_base.error("   FTP Upload of file %s failed as file could not be created on FPP.", (const char *)file.c_str());
        }
    }
    else
    {
        wxMessageBox("FTP Upload of file failed to open the file.");
        progress.Update(100, wxEmptyString, &cancelled);
        logger_base.error("   FTP Upload of file %s failed as file could not be opened.", (const char *)file.c_str());
    }

    if (!cancelled)
    {
        cancelled = progress.WasCancelled();
    }

    return cancelled;
}

SimpleFTP::~SimpleFTP()
{
    ftp.Close();
}
