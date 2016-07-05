#include "DmxModel.h"
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/xml/xml.h>
#include "ModelScreenLocation.h"
#include "ModelPreview.h"
#include "../RenderBuffer.h"

DmxModel::DmxModel(wxXmlNode *node, const ModelManager &manager, bool zeroBased) : ModelWithScreenLocation(manager)
{
    SetFromXml(node, zeroBased);
}
DmxModel::DmxModel(const ModelManager &manager) : ModelWithScreenLocation(manager)
{
    //ctor
}

DmxModel::~DmxModel()
{
    //dtor
}

const double PI  =3.141592653589793238463;
#define ToRadians(x) ((double)x * PI / (double)180.0)

class dmxPoint {

public:
    float x;
    float y;

    dmxPoint( float x_, float y_, int cx_, int cy_, float scale_, float angle_ )
    : x(x_), y(y_), cx(cx_), cy(cy_), scale(scale_)
    {
        float s = RenderBuffer::sin(ToRadians(angle_));
        float c = RenderBuffer::cos(ToRadians(angle_));

        // scale point
        x *= scale;
        y *= scale;

        // rotate point
        float xnew = x * c - y * s;
        float ynew = x * s + y * c;

        // translate point
        x = xnew + cx;
        y = ynew + cy;
    }

private:
    float cx;
    float cy;
    float scale;
};

static wxPGChoices DMX_STYLES;

void DmxModel::GetBufferSize(const std::string &type, const std::string &transform, int &BufferWi, int &BufferHi) const {
        BufferHi = 1;
        BufferWi = GetNodeCount();
}
void DmxModel::InitRenderBufferNodes(const std::string &type,  const std::string &transform,
                                     std::vector<NodeBaseClassPtr> &newNodes, int &BufferWi, int &BufferHi) const {
    BufferHi = 1;
    BufferWi = GetNodeCount();

    for (int cur=0; cur < BufferWi; cur++) {
        newNodes.push_back(NodeBaseClassPtr(Nodes[cur]->clone()));
        for(size_t c=0; c < newNodes[cur]->Coords.size(); c++) {
            newNodes[cur]->Coords[c].bufX=cur;
            newNodes[cur]->Coords[c].bufY=0;
        }
    }
}

void DmxModel::AddTypeProperties(wxPropertyGridInterface *grid) {
    if (DMX_STYLES.GetCount() == 0) {
        DMX_STYLES.Add("Moving Head Top");
        DMX_STYLES.Add("Moving Head Side");
    }

    AddStyleProperties(grid);

    wxPGProperty *p = grid->Append(new wxUIntProperty("# Channels", "DmxChannelCount", parm1));
    p->SetAttribute("Min", 1);
    p->SetAttribute("Max", 512);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Pan Channel", "DmxPanChannel", pan_channel));
    p->SetAttribute("Min", 1);
    p->SetAttribute("Max", 512);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Pan Orientation", "DmxPanOrient", pan_orient));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 360);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Pan Deg of Rot", "DmxPanDegOfRot", pan_deg_of_rot));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 1000);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Tilt Channel", "DmxTiltChannel", tilt_channel));
    p->SetAttribute("Min", 1);
    p->SetAttribute("Max", 512);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Tilt Orientation", "DmxTiltOrient", tilt_orient));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 360);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Tilt Deg of Rot", "DmxTiltDegOfRot", tilt_deg_of_rot));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 1000);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Red Channel", "DmxRedChannel", red_channel));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 512);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Green Channel", "DmxGreenChannel", green_channel));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 512);
    p->SetEditor("SpinCtrl");

    p = grid->Append(new wxUIntProperty("Blue Channel", "DmxBlueChannel", blue_channel));
    p->SetAttribute("Min", 0);
    p->SetAttribute("Max", 512);
    p->SetEditor("SpinCtrl");

}
void DmxModel::AddStyleProperties(wxPropertyGridInterface *grid) {
    grid->Append(new wxEnumProperty("DMX Style", "DmxStyle", DMX_STYLES, dmx_style == "Moving Head Top" ? 0 : 1));
}

int DmxModel::OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) {
    if ("DmxStyle" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxStyle");
        ModelXml->AddAttribute("DmxStyle", event.GetPropertyValue().GetLong() ? "Moving Head Side" : "Moving Head Top");
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxChannelCount" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("parm1");
        ModelXml->AddAttribute("parm1", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3 | 0x0008;
    } else if ("DmxPanChannel" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxPanChannel");
        ModelXml->AddAttribute("DmxPanChannel", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxPanOrient" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxPanOrient");
        ModelXml->AddAttribute("DmxPanOrient", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxPanDegOfRot" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxPanDegOfRot");
        ModelXml->AddAttribute("DmxPanDegOfRot", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxTiltChannel" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxTiltChannel");
        ModelXml->AddAttribute("DmxTiltChannel", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxTiltOrient" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxTiltOrient");
        ModelXml->AddAttribute("DmxTiltOrient", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxTiltDegOfRot" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxTiltDegOfRot");
        ModelXml->AddAttribute("DmxTiltDegOfRot", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxRedChannel" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxRedChannel");
        ModelXml->AddAttribute("DmxRedChannel", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxGreenChannel" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxGreenChannel");
        ModelXml->AddAttribute("DmxGreenChannel", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    } else if ("DmxBlueChannel" == event.GetPropertyName()) {
        ModelXml->DeleteAttribute("DmxBlueChannel");
        ModelXml->AddAttribute("DmxBlueChannel", wxString::Format("%d", event.GetPropertyValue().GetLong()));
        SetFromXml(ModelXml, zeroBased);
        return 3;
    }

    return Model::OnPropertyGridChange(grid, event);
}

void DmxModel::InitModel() {
    DisplayAs = "DMX";
    StringType=="Single Color White";
    parm2 = 1;
    parm3 = 1;

    int numChannels = parm1;
    SetNodeCount(numChannels, 1, rgbOrder);

    int width = 0;
    int curNode = 0;
    for (int x = 0; x < numChannels; x++) {
        Nodes[curNode]->ActChan = stringStartChan[0] + curNode*GetNodeChannelCount(StringType);
        Nodes[curNode]->StringNum=0;
        Nodes[curNode]->Coords[0].bufX = 0;
        Nodes[curNode]->Coords[0].bufY = 0;
        Nodes[curNode]->Coords[0].screenX = 0;
        Nodes[curNode]->Coords[0].screenY = 0;
        curNode++;
    }
    SetBufferSize(1,parm1);
    screenLocation.SetRenderSize(1, 1);

	dmx_style = ModelXml->GetAttribute("DmxStyle", "Moving Head Top");
	pan_channel = wxAtoi(ModelXml->GetAttribute("DmxPanChannel", "1"));
	pan_orient = wxAtoi(ModelXml->GetAttribute("DmxPanOrient", "0"));
	pan_deg_of_rot = wxAtoi(ModelXml->GetAttribute("DmxPanDegOfRot", "540"));
	tilt_channel = wxAtoi(ModelXml->GetAttribute("DmxTiltChannel", "1"));
	tilt_orient = wxAtoi(ModelXml->GetAttribute("DmxTiltOrient", "0"));
	tilt_deg_of_rot = wxAtoi(ModelXml->GetAttribute("DmxTiltDegOfRot", "180"));
	red_channel = wxAtoi(ModelXml->GetAttribute("DmxRedChannel", "0"));
	green_channel = wxAtoi(ModelXml->GetAttribute("DmxGreenChannel", "0"));
	blue_channel = wxAtoi(ModelXml->GetAttribute("DmxBlueChannel", "0"));
}

void DmxModel::DisplayEffectOnWindow(ModelPreview* preview, double pointSize)
{
    bool success = preview->StartDrawing(pointSize);

    if(success) {
        DrawGLUtils::xlAccumulator va(maxVertexCount);

        size_t NodeCount=Nodes.size();
        float sx,sy;
        xlColor color, proxy;
        int w, h;

        GetModelScreenLocation().PrepareToDraw();

        va.PreAlloc(maxVertexCount);

        preview->GetSize(&w, &h);

        sx=w/2;
        sy=h/2;

        DrawModelOnWindow(preview, va, nullptr, sx, sy, true);

        DrawGLUtils::Draw(va);

        preview->EndDrawing();

    }

}

// display model using colors
void DmxModel::DisplayModelOnWindow(ModelPreview* preview, DrawGLUtils::xlAccumulator &va, const xlColor *c, bool allowSelected) {
    float sx,sy;
    int w, h;
    preview->GetVirtualCanvasSize(w, h);

    GetModelScreenLocation().PrepareToDraw();

    va.PreAlloc(maxVertexCount);

    sx=0;
    sy=0;
    GetModelScreenLocation().TranslatePoint(sx, sy);

    DrawModelOnWindow(preview, va, c, sx, sy, !allowSelected);

    if (Selected && c != NULL && allowSelected) {
        GetModelScreenLocation().DrawHandles(va);
    }
}

void DmxModel::DrawModelOnWindow(ModelPreview* preview, DrawGLUtils::xlAccumulator &va, const xlColor *c, float &sx, float &sy, bool active)
{
    float angle, pan_angle, tilt_angle, angle1, angle2, beam_length;
    int x1, x2, y1, y2;
    size_t NodeCount=Nodes.size();
    bool beam_off = false;

    if( pan_channel > NodeCount ||
        tilt_channel > NodeCount ||
        red_channel > NodeCount ||
        green_channel > NodeCount ||
        blue_channel > NodeCount )
    {
        return;
    }

    xlColor ccolor(xlWHITE);
    xlColor pnt_color(xlRED);
    xlColor beam_color(xlWHITE);
    xlColor marker_color(xlBLACK);
    xlColor base_color(200, 200, 200);
    xlColor color;
    if (c != NULL) {
        color = *c;
    }

    bool top_view = (dmx_style == "Moving Head Top" );

    int dmx_size = ((BoxedScreenLocation)screenLocation).GetScaleX() * screenLocation.previewW;
    float radius = (float)(dmx_size) / 2.0f;
    xlColor color_angle;

    int trans = color == xlBLACK ? blackTransparency : transparency;
    if( red_channel > 0 && green_channel > 0 && blue_channel > 0 ) {
        xlColor proxy;
        Nodes[red_channel-1]->GetColor(proxy);
        beam_color.red = proxy.red;
        Nodes[green_channel-1]->GetColor(proxy);
        beam_color.green = proxy.red;
        Nodes[blue_channel-1]->GetColor(proxy);
        beam_color.blue = proxy.red;
    }
    if( beam_color.red == 0 && beam_color.green == 0 && beam_color.blue == 0 || !active ) {
        beam_color = xlWHITE;
        beam_off = true;
    } else {
        ApplyTransparency(beam_color, trans);
        marker_color = beam_color;
    }
    ApplyTransparency(ccolor, trans);
    ApplyTransparency(base_color, trans);
    ApplyTransparency(pnt_color, trans);

    Nodes[pan_channel-1]->GetColor(color_angle);
    pan_angle = (color_angle.red / 255.0f) * pan_deg_of_rot + pan_orient;
    Nodes[tilt_channel-1]->GetColor(color_angle);
    tilt_angle = (color_angle.red / 255.0f) * tilt_deg_of_rot + tilt_orient;

    // Determine if we need to flip the beam
    int tilt_pos = (int)(RenderBuffer::cos(ToRadians(tilt_angle))*radius*0.8);
    if( tilt_pos < 0 ) {
        if( pan_angle >= 180.0f ) {
            pan_angle -= 180.0f;
        } else {
            pan_angle += 180.0f;
        }
        tilt_pos *= -1;
    }

    if( top_view ) {
        angle = pan_angle;
    } else {
        angle = tilt_angle;
    }

    float beam_width = 30.0f;
    beam_length = radius * 4.0f;
    angle1 = angle - beam_width / 2.0f;
    angle2 = angle + beam_width / 2.0f;
    if( angle1 < 0.0f ) {
        angle1 += 360.0f;
    }
    if( angle2 > 360.f ) {
        angle2 -= 360.0f;
    }
    x1 = (int)(RenderBuffer::cos(ToRadians(angle1))*beam_length);
    y1 = (int)(RenderBuffer::sin(ToRadians(angle1))*beam_length);
    x2 = (int)(RenderBuffer::cos(ToRadians(angle2))*beam_length);
    y2 = (int)(RenderBuffer::sin(ToRadians(angle2))*beam_length);

    // Draw the light beam
    va.AddVertex(sx, sy, beam_color);
    ApplyTransparency(beam_color, beam_off ? 0 : 100);
    va.AddVertex(sx+x1, sy+y1, beam_color);
    va.AddVertex(sx+x2, sy+y2, beam_color);

    float scale = radius / 12.0f;
    if( top_view ) {
        va.AddTrianglesCircle(sx, sy, radius, ccolor, ccolor);

        // draw angle line
        dmxPoint p1(0, -1, sx, sy, scale, angle);
        dmxPoint p2(12, -1, sx, sy, scale, angle);
        dmxPoint p3(12, 1, sx, sy, scale, angle);
        dmxPoint p4(0, 1, sx, sy, scale, angle);

        va.AddVertex(p1.x, p1.y, pnt_color);
        va.AddVertex(p2.x, p2.y, pnt_color);
        va.AddVertex(p3.x, p3.y, pnt_color);

        va.AddVertex(p1.x, p1.y, pnt_color);
        va.AddVertex(p3.x, p3.y, pnt_color);
        va.AddVertex(p4.x, p4.y, pnt_color);

        // draw tilt marker
        dmxPoint marker(tilt_pos, 0, sx, sy, 1.0, angle);
        va.AddTrianglesCircle(marker.x, marker.y, radius*0.20, marker_color, marker_color);
    } else {
        // draw head
        dmxPoint p1(12, -13, sx, sy, scale, angle);
        dmxPoint p2(12, +13, sx, sy, scale, angle);
        dmxPoint p3(-12, +10, sx, sy, scale, angle);
        dmxPoint p4(-15, +5, sx, sy, scale, angle);
        dmxPoint p5(-15, -5, sx, sy, scale, angle);
        dmxPoint p6(-12, -10, sx, sy, scale, angle);

        va.AddVertex(p1.x, p1.y, ccolor);
        va.AddVertex(p2.x, p2.y, ccolor);
        va.AddVertex(p6.x, p6.y, ccolor);

        va.AddVertex(p2.x, p2.y, ccolor);
        va.AddVertex(p3.x, p3.y, ccolor);
        va.AddVertex(p6.x, p6.y, ccolor);

        va.AddVertex(p3.x, p3.y, ccolor);
        va.AddVertex(p5.x, p5.y, ccolor);
        va.AddVertex(p6.x, p6.y, ccolor);

        va.AddVertex(p3.x, p3.y, ccolor);
        va.AddVertex(p4.x, p4.y, ccolor);
        va.AddVertex(p5.x, p5.y, ccolor);

        // draw base
        va.AddTrianglesCircle(sx, sy, radius*0.6, base_color, base_color);
        va.AddRect(sx-radius*0.6, sy, sx+radius*0.6, sy-radius*2, base_color);

        // draw pan marker
        dmxPoint p7(7, 2, sx, sy, scale, pan_angle);
        dmxPoint p8(7, -2, sx, sy, scale, pan_angle);
        va.AddVertex(sx, sy, marker_color);
        va.AddVertex(p7.x, p7.y, marker_color);
        va.AddVertex(p8.x, p8.y, marker_color);
    }

    va.Finish(GL_TRIANGLES);
}
