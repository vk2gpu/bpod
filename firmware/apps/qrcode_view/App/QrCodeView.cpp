#include "QrCodeView.hpp"

#include <Images/BpodTitleBar.hpp>

#include <qrcodegen.h>

void QrCodeView::begin(void)
{
    redraw_ = true;
    qrcode_mem_ = new uint8_t[QR_MEM_SIZE];
    qrcode_temp_mem_ = new uint8_t[QR_MEM_TEMP_SIZE];
}

void QrCodeView::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void QrCodeView::draw_title(Adafruit_GFX &gfx)
{
    int16_t x1 = 0;
    int16_t y1 = 0;
    uint16_t w = 0;
    uint16_t h = 0;
    gfx.setTextColor(0x0000);
    gfx.setTextSize(2);
    gfx.getTextBounds(title_.c_str(), 0, 0, &x1, &y1, &w, &h);
    BpodTitleBar::draw(0, 0, gfx.width(), gfx);
    if ( title_.size() > 0 )
    {
        x1 = (gfx.width() / 2) - (w / 2);
        y1 = (BpodTitleBar::height / 2) - (h / 2);
        gfx.setCursor(x1, y1);
        gfx.print(title_.c_str());
    }
}

void QrCodeView::draw_qrcode(Adafruit_GFX &gfx)
{
    bool ok = qrcodegen_encodeText(text_.c_str(), qrcode_temp_mem_, qrcode_mem_, qrcodegen_Ecc_LOW,
        qrcodegen_MAX_VERSION, qrcodegen_MAX_VERSION, qrcodegen_Mask_0, true);
    //if (!ok) {
    //    return;
    //}
    int size = qrcodegen_getSize(qrcode_mem_);
    int x_offset = (gfx.width() / 2) - ((size * 2) / 2);
    int y_offset = BpodTitleBar::height + ((gfx.height() - BpodTitleBar::height) / 2) - ((size * 2) / 2);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            gfx.fillRect(x_offset + (x * 2), y_offset + (y * 2), 2, 2, qrcodegen_getModule(qrcode_mem_, x, y) ? 0x0000 : 0xffff);
        }
    }
}

void QrCodeView::draw(Adafruit_GFX &gfx)
{
    if ( redraw_ )
    {
        gfx.fillScreen(0xffff);
        draw_title(gfx);
        draw_qrcode(gfx);
        redraw_ = false;
    }
}

void QrCodeView::end(void)
{
    delete qrcode_mem_;
    delete qrcode_temp_mem_;
    qrcode_mem_ = nullptr;
    qrcode_temp_mem_ = nullptr;
}
