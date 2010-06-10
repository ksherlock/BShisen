#include "Tiles.h"
#include <TranslationUtils.h>
#include <Alert.h>
#include <Application.h>


Tiles::Tiles(void):
	tiles_std(NULL),
	tiles_custom(NULL),
	use_custom(false)
	
{
register int i;

	tiles_std = new BBitmap *[36];
	tiles_custom = new BBitmap *[36];
	for (i = 0; i < 36; i++)
	{
		tiles_std[i] = NULL;
		tiles_custom[i] = NULL;
	}
#if 0
	tiles[0] = BTranslationUtils::GetBitmap("pic1.bmp");
	tiles[1] = BTranslationUtils::GetBitmap("pic2.bmp"); 
	tiles[2] = BTranslationUtils::GetBitmap("pic3.bmp");
	tiles[3] = BTranslationUtils::GetBitmap("pic4.bmp");
	tiles[4] = BTranslationUtils::GetBitmap("pic5.bmp");
	tiles[5] = BTranslationUtils::GetBitmap("pic6.bmp");
	tiles[6] = BTranslationUtils::GetBitmap("pic7.bmp");
	tiles[7] = BTranslationUtils::GetBitmap("pic8.bmp");
	tiles[8] = BTranslationUtils::GetBitmap("pic9.bmp");
	tiles[9] = BTranslationUtils::GetBitmap("pic10.bmp");
	tiles[10] = BTranslationUtils::GetBitmap("pic11.bmp");
	tiles[11] = BTranslationUtils::GetBitmap("pic12.bmp");
	tiles[12] = BTranslationUtils::GetBitmap("pic13.bmp");
	tiles[13] = BTranslationUtils::GetBitmap("pic14.bmp");
	tiles[14] = BTranslationUtils::GetBitmap("pic15.bmp");
	tiles[15] = BTranslationUtils::GetBitmap("pic16.bmp");
	tiles[16] = BTranslationUtils::GetBitmap("pic17.bmp");
	tiles[17] = BTranslationUtils::GetBitmap("pic18.bmp");
	tiles[18] = BTranslationUtils::GetBitmap("pic19.bmp");
	tiles[19] = BTranslationUtils::GetBitmap("pic20.bmp");
	tiles[20] = BTranslationUtils::GetBitmap("pic21.bmp");
	tiles[21] = BTranslationUtils::GetBitmap("pic22.bmp");
	tiles[22] = BTranslationUtils::GetBitmap("pic23.bmp");
	tiles[23] = BTranslationUtils::GetBitmap("pic24.bmp");
	tiles[24] = BTranslationUtils::GetBitmap("pic25.bmp");
	tiles[25] = BTranslationUtils::GetBitmap("pic26.bmp");
	tiles[26] = BTranslationUtils::GetBitmap("pic27.bmp");
	tiles[27] = BTranslationUtils::GetBitmap("pic28.bmp");
	tiles[28] = BTranslationUtils::GetBitmap("pic29.bmp");
	tiles[29] = BTranslationUtils::GetBitmap("pic30.bmp");
	tiles[30] = BTranslationUtils::GetBitmap("pic31.bmp");
	tiles[31] = BTranslationUtils::GetBitmap("pic32.bmp");
	tiles[32] = BTranslationUtils::GetBitmap("pic33.bmp");
	tiles[33] = BTranslationUtils::GetBitmap("pic34.bmp");
	tiles[34] = BTranslationUtils::GetBitmap("pic35.bmp");
	tiles[35] = BTranslationUtils::GetBitmap("pic36.bmp");
	#endif
#if 0
	for (i = 5; i < 36; i++)
		tiles[i] = BTranslationUtils::GetBitmap("pic1.bmp"); 
#endif

	for (i = 0; i < 36; i++)
	{
		tiles_std[i] = BTranslationUtils::GetBitmap(B_RAW_TYPE, i+1);
	}
		// if any are NULL, give alert & QUIT
	for (i = 0; i < 36; i++)
	{
		if (tiles_std[i] == NULL)
		{
			BAlert *b;
			b = new BAlert("", "Error: This game is corrupt. Please "
								"either reinstall or redownload.  "
								"Furthermore, stop sector editing me!",
								"Ok");
			b->Go();
			be_app->PostMessage(B_QUIT_REQUESTED);
		
		}
	
	}

}

Tiles::~Tiles(void)
{
register unsigned int i;

	for (i = 0; i < 36; i++)
	{
		if (tiles_std[i]) delete tiles_std[i];
		if (tiles_custom[i]) delete tiles_custom[i];
	}
	delete tiles_std;
	delete tiles_custom;
}
#include <Menu.h>
#include <MenuItem.h>
#include <Directory.h>
#include <Path.h>
#include <Application.h>
#include <Entry.h>
#include <Message.h>
#include <Roster.h>

#define TILES_MITEM 'elit'
int Tiles::MakeTileMenu(BMenu *menu)
{
//	menu->AddItem(new BMenuItem("test", NULL));

struct app_info info;
BEntry e;
BEntry e2;
BPath p;
BDirectory d;
//char name[B_FILE_NAME_LENGTH];
int cnt;
BMenuItem *m;
BMessage *msg;
	cnt = 0;
	//if (be_roster->GetActiveAppInfo(&info) != B_OK) return;
	if (be_app->GetAppInfo(&info) != B_OK) return 0;
	e.SetTo(&info.ref, true);
	e.GetParent(&e2);
	e2.GetPath(&p);	
	p.Append("Tiles");
	if (d.SetTo(p.Path()) != B_OK) return 0;
	//printf("%s\n",p.Path());


	while (d.GetNextEntry(&e) == B_OK)
	{
		cnt++;
		//e.GetName(name);
		e.GetPath(&p);
		msg = new BMessage(TILES_MITEM);
		msg->AddString("Pathname",p.Path());

		m = new BMenuItem(p.Leaf(), msg);
		menu->AddItem(m);
//		printf("%s\n", name);
	}
	return cnt;
}

#include <DataIO.h>
#include <File.h>
#include <Resources.h>