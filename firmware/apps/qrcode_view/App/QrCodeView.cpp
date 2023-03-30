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

void QrCodeView::draw(Adafruit_GFX &gfx)
{
    if ( redraw_ )
    {
        BpodTitleBar::draw(gfx, title_);
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        qrcodegen_encodeText(text_.c_str(), qrcode_temp_mem_, qrcode_mem_, qrcodegen_Ecc_LOW,
            qrcodegen_MAX_VERSION, qrcodegen_MAX_VERSION, qrcodegen_Mask_0, true);
        int size = qrcodegen_getSize(qrcode_mem_);
        int x_offset = (gfx.width() / 2) - ((size * 2) / 2);
        int y_offset = BpodTitleBar::view_y(gfx) + ((gfx.height() - BpodTitleBar::view_y(gfx)) / 2) - ((size * 2) / 2);
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                gfx.fillRect(x_offset + (x * 2), y_offset + (y * 2), 2, 2, qrcodegen_getModule(qrcode_mem_, x, y) ? 0x0000 : 0xffff);
            }
        }
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
