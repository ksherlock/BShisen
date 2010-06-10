#include "Shisen.h"
#include "GameWindow.h"
#include <View.h>
#include <StringView.h>
#include <TranslationUtils.h>
#include <OS.h>
#include <Entry.h>
#include "res.h"

/*
 * $Id: Shisen.cpp,v 1.3 1999/07/09 05:21:55 baron Exp baron $
 */

int REGISTERED;
int NAG;
uint64 KEY;

union split
{
	uint64 i64;
	uint32 i32[2];
	uint16 i16[4];
	uint8 i8[8];
};

Shisen::Shisen():
	BApplication("application/x-vnd.KWS-BShisen")
{
system_info info;
union split sp;
app_info app;


	REGISTERED = false;
	NAG = false;
	
	get_system_info(&info);
	
	if (info.id[0] && info.id[1]) // P3 ?
	{
		sp.i32[0] = info.id[0];
		sp.i32[1] = info.id[1];
		KEY = sp.i64;
	}
	else	// make a KEY from the CPU TYPE & the speed
	{
		KEY = info.cpu_clock_speed;
		KEY |= info.cpu_revision;
	}
	
	//printf("key is %lx\n", KEY);
	
	
	/*
	 * 1) Check the creation time
	 * 2) Check the last accessed time
	 * 3) subtract
	 * if > 3 days, nag++
	 */
	
	if (GetAppInfo(&app) == B_OK)
	{
		BEntry entry;
		struct stat st;
		time_t t_t;
		
		entry.SetTo(&app.ref, true);
		entry.GetStat(&st);
	
		t_t = st.st_atime - st.st_ctime;

		if (t_t > 5 * 24 * 60 * 60)
			NAG = true;
	}

	game = new GameWindow;
	game->Show();	
}

Shisen::~Shisen()
{
}


void Shisen::AboutRequested(void)
{
BWindow *about = NULL;
thread_id id;


	id = find_thread("w>About BShisen");
	if (id != B_NAME_NOT_FOUND)
	{
		about = (BWindow *)LooperForThread(id);
	}

	if (about)
	{
		about->SetWorkspaces(B_CURRENT_WORKSPACE);
		about->Activate();
	}
	else
	{
		BView *back;
		BStringView *str;
		BBitmap *bmap;
		
		about = new BWindow(BRect(50, 50, 360, 170), "About BShisen",
			B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE);
		
		back = new BView(about->Bounds(), NULL, B_FOLLOW_NONE, B_WILL_DRAW);
		back->SetViewColor(0, 0, 0);	//black background
		about->AddChild(back);
	
	
		str = new BStringView(BRect(100, 10, 310, 30), NULL, "BShisen 1.1");
		str->SetFontSize(12);
		str->SetHighColor(0,255,0);
		back->AddChild(str);
	
		str = new BStringView(BRect(100, 40-5, 310, 60-5), NULL,
			"(c) 1998, 1999 Kelvin W Sherlock");
		str->SetFontSize(12);
		str->SetHighColor(0,255,0);
		back->AddChild(str);
	
		str = new BStringView(BRect(100, 70-10, 310, 90-10), NULL,
			"kws@delphi.com");
		str->SetFontSize(12);
		str->SetHighColor(0,255,0);
		back->AddChild(str);
	
		if (!REGISTERED) //70, 90
		{
			str = new BStringView(BRect(100, 100-15, 310, 120-15), NULL, "UNREGISTERED - PLEASE REGISTER");
			str->SetFontSize(12);
			str->SetHighColor(255,0,0);
			back->AddChild(str);
		}
		//bmap = BTranslationUtils::GetBitmap("S.tiff");
		
		bmap = BTranslationUtils::GetBitmap(B_RAW_TYPE, ABOUT_PIC_ID);
		if (bmap)
		{
			back->SetViewBitmap(bmap, B_FOLLOW_ALL, 0);
			free(bmap);
		}
		about->Flush();
		about->Show();	
	}
	/* Add window controls */
}
