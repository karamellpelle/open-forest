#ifndef _DLGOPTIONS_H
#define _DLGOPTIONS_H

class DlgOptions : public DlgBase
{
 public:
    
    void reshape(int x, int y, int width, int height);

    DlgOptions(DlgBase* parent, int x, int y, puCallback ok_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb);
    }

    virtual ~DlgOptions();
    void print() { std::cout << "DlgMenu\n"; }

 private:
    puOneShot* window_button;
    puOneShot* fullscreen_640x480_button;
    puOneShot* fullscreen_800x600_button;
    puOneShot* fullscreen_1024x800_button;
    puOneShot* fullscreen_1280x1024_button;
    puOneShot* ok_button;

    static void cancel_cb(puObject *btn);
    static void single_start_cb(puObject *btn);

    static void window_cb(puObject *);
    static void fullscreen_640x480_cb(puObject *);
    static void fullscreen_800x600_cb(puObject *);
    static void fullscreen_1024x800_cb(puObject *);
    static void fullscreen_1280x1024_cb(puObject *);
    static void ok_cb(puObject *);

    void init(puCallback ok_cb);
};

#endif // _DLGOPTIONS_H
