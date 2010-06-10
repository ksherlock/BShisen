#include "Prefs.h"

#include <Path.h>
#include <FindDirectory.h>
#include <Node.h>
#include <File.h>
#include <fs_attr.h>
#include <View.h>
#include <Button.h>
#include <StringView.h>
#include <Application.h>
#include <Box.h>

Prefs::Prefs():
	Changed(false)
{
BNode N;
struct attr_info Info;


	tile = false;
	back_color.red = 216;
	back_color.green = 216;
	back_color.blue = 216;
	back_color.alpha = 0;
	key = 0;
	fresh = false;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_NO_ERROR
		&& path.Append(PREFS_FILE_NAME, true) == B_NO_ERROR
		&& N.SetTo(path.Path()) == B_NO_ERROR)
	{
		N.Lock();
	
		if (N.GetAttrInfo(PIC_PATH, &Info) == B_NO_ERROR
			&& Info.type == B_STRING_TYPE)
		{
			char *buffer = new char[Info.size];
			N.ReadAttr(PIC_PATH, B_STRING_TYPE, 0, buffer, Info.size);
			
			pic_path.SetTo(buffer, Info.size);
			delete []buffer;

		}
		
		if (N.GetAttrInfo(TILE_PIC, &Info) == B_NO_ERROR
			&& Info.type == B_INT32_TYPE)
		{
			N.ReadAttr(TILE_PIC, B_INT32_TYPE, 0, &tile, 4);
		}
	
		if (N.GetAttrInfo(BACK_COLOR, &Info) == B_NO_ERROR
			&& Info.type == B_RGB_COLOR_TYPE)
		{
			N.ReadAttr(BACK_COLOR, B_RGB_COLOR_TYPE, 0, &back_color,
				sizeof(struct rgb_color));
		}
	
		if (N.GetAttrInfo(KEY_PHRASE, &Info) == B_NO_ERROR
			&& Info.type == B_UINT64_TYPE)
		{
			N.ReadAttr(KEY_PHRASE, B_UINT64_TYPE, 0, &key,
				sizeof(uint64));
		}
	
		N.Unlock();
	}
	else fresh = true;
}



Prefs::~Prefs()
{
}

void Prefs::WritePrefs(void)
{

	//if (!Changed) return;

	BNode N(path.Path());
	BFile F(path.Path(), B_WRITE_ONLY | B_CREATE_FILE);
	
	if (F.InitCheck() != B_OK)	//guarantee that the file exists
		return;
	
	F.Unset();
	
	N.Lock();
	//N.RemoveAttr(PIC_PATH);
	//N.RemoveAttr(TILE_PIC);
	//N.RemoveAttr(BACK_COLOR);
	
	N.WriteAttr(PIC_PATH, B_STRING_TYPE, 0, pic_path.String(),
		pic_path.Length());	// + 1 makes it include NULL 
	
	N.WriteAttr(TILE_PIC, B_INT32_TYPE, 0, &tile, 4);
	
	N.WriteAttr(BACK_COLOR, B_RGB_COLOR_TYPE, 0, &back_color,
		sizeof(struct rgb_color));
	// don't write if not registered 
	if (key)
	{
		N.WriteAttr(KEY_PHRASE, B_UINT64_TYPE, 0, &key, sizeof(uint64));
	}
	
	N.Unlock();
}

bool Prefs::GetTilePic() const
{
	return tile;
}
const char *Prefs::GetPicPath(void) const
{
	return pic_path.String();
}
const rgb_color& Prefs::GetBackColor(void) const
{
	return back_color;
}
uint64 Prefs::GetKey(void) const
{
	return key;
}
void Prefs::SetKey(uint64 k)
{
	key = k;
}
void Prefs::SetTilePic(bool b)
{
	tile = b;
	Changed = true;
}
void Prefs::SetPicPath(const char *p)
{
	pic_path.SetTo(p);
	Changed = true;
}
void Prefs::SetBackColor(const rgb_color& c)
{
	back_color = c;
	Changed = true;
}

void Prefs::Go(BLooper *looper)
{
//bool ret_val;
PrefEdit *P;


	P = new PrefEdit(this, looper);
	P->Show();
	
	//ret_val = P->Go();
	//be_app->SetCursor(B_HAND_CURSOR);
	//P->Lock();
	//P->Quit();
	
	//return ret_val;
}


PrefEdit::PrefEdit(Prefs *P, BLooper *l):
	BWindow(BRect(100, 100, 470, 350), "BShisen Preferences",
		B_TITLED_WINDOW,	
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_NOT_MINIMIZABLE),
		prefs(P)
{
BView *V;
BButton *B;
BBox *box;

	loop = l;
	//RemoveShortcut('q', B_COMMAND_KEY);
	V = new BView(Bounds(), "back", B_FOLLOW_ALL, B_WILL_DRAW);
	V->SetViewColor(216, 216, 216);
	AddChild(V);
	
	btxt = new BTextControl(BRect(10, 10, 360, 25), "", "Background Picture",
		NULL, NULL);
	btxt->SetText(prefs->GetPicPath());
	btxt->SetDivider(btxt->StringWidth("Background Picture "));	
	
	
	
	V->AddChild(btxt);
	
	cbox = new BCheckBox(BRect(10, 35, 200, 50), "", "Tile Picture", NULL);
	
	cbox->SetValue(prefs->GetTilePic());	
	V->AddChild(cbox);
	
	box = new BBox(BRect(10, 60, 360, 200));
	box->SetLabel("Background Color");
	V->AddChild(box);
	
	color_sample = new BView(BRect(10, 15, 40, 45), "",	B_FOLLOW_ALL, B_WILL_DRAW);
	color_sample->SetViewColor(prefs->GetBackColor());
	box->AddChild(color_sample);
	//color_sample->Invalidate();
	//color_sample->Draw(BRect(10,15,40,45));
	
	cctl = new BColorControl(BPoint(10, 50/*10+10, 120*/), B_CELLS_32x8, 8, "",
			new BMessage('cccc'));
	cctl->SetValue(prefs->GetBackColor());
	box->AddChild(cctl);
	
	B = new BButton(BRect(10, 160+50, 80, 190+50), "", "Accept", new BMessage('acce'));
	B->MakeDefault(true);
	V->AddChild(B);
	B = new BButton(BRect(90, 160+50, 160, 190+50), "", "Cancel", new BMessage('canc'));
	V->AddChild(B);
}

PrefEdit::~PrefEdit()
{
}
void PrefEdit::MessageReceived(BMessage *msg)
{
BMessage *bm;

	switch(msg->what)
	{
	case 'cccc':
		color_sample->SetViewColor(cctl->ValueAsColor());
		color_sample->Invalidate();
		break;
//			uint32 new_val;
//			
//			if (msg->FindInt32("be:value", (int32 *)&new_val) == B_OK)
//			{
//				color_sample->SetHighColor(new_val);
//				color_sample->Invalidate();
//			}
//			//msg->PrintToStream();
//		}
//		break;
	case 'acce':
		prefs->SetTilePic(cbox->Value());
		prefs->SetPicPath(btxt->Text());
		prefs->SetBackColor(cctl->ValueAsColor());
		//write_port(Port, 1, NULL, 0);
		be_app->SetCursor(B_HAND_CURSOR);
		bm = new BMessage('nopa');
		bm->AddInt32("Prefs", true);
		loop->PostMessage(bm);
		Quit();
		break;
		
	case 'canc':
		//write_port(Port, 0, NULL, 0);
		be_app->SetCursor(B_HAND_CURSOR);
		loop->PostMessage(new BMessage('nopa'));
		Quit();
		break;
		
	default: BWindow::MessageReceived(msg);
	}

}

bool PrefEdit::QuitRequested(void)
{
	be_app->SetCursor(B_HAND_CURSOR);
	loop->PostMessage(new BMessage('nopa'));
	return true;
}

#if 0
bool PrefEdit::Go(void)
{
int32 retval;

	Port = create_port(1, "Preferences port");
	Show();
	read_port(Port, &retval, NULL, 0);
	Hide();
	delete_port(Port);
	return retval;
}
#endif