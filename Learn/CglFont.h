#pragma once
class CglFont {
public:
    CglFont();
    void SetColor(float r, float g, float b);
    void SetPosition(int x, int y);
    void SetFont(void* glutFont);
    void DrawString(const std::string& text) const; 

private:
    float m_color[3];
    int m_posX;
    int m_posY;
    void* m_font;
};




