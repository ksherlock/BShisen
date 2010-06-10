#ifndef __PREFS_H__
#define __PREFS_H__

/*
 * class to load/store/edit/retrieve preferenecs
 *
 * Copyright 1999 Kelvin W Sherlock
 *
 * $Id: Prefs.h,v 1.1 1999/03/18 00:35:58 baron Exp baron $
 */

#include <Window.h>
#include <OS.h>
#include <String.h>
#include <Path.h>
#include <CheckBox.h>
#include <TextControl.h>
#include <ColorControl.h>

#define PREFS_FILE_NAME "BShisen.Prefs"

#define PIC_PATH	"Background Picture"
#define TILE_PIC	"Tile Picture"
#define BACK_COLOR	"Background Color"
#define KEY_PHRASE	"Big Brother"

class Prefs
{
public:

	Prefs();
	~Prefs();
	

void	Go(BLooper *);	//returns TRUE if prefs changed

bool				IsFresh() const {return fresh;};

bool				GetTilePic() const;
const char *		GetPicPath() const;
const rgb_color&	GetBackColor() const;
uint64				GetKey() const;

void				SetTilePic(bool);
void				SetPicPath(const char *);
void				SetBackColor(const rgb_color&);
void				SetKey(uint64 k);

void WritePrefs(void);


private:

BString pic_path;		// path to background picture
int tile;				//tile the background pic?
rgb_color back_color;	//backgorund color

BPath path;		//path of preferences
uint64 key;

bool Changed;
bool fresh;


};

class PrefEdit: public BWindow
{
public:
	PrefEdit(Prefs *, BLooper *);
	~PrefEdit();
	
//bool	Go();
virtual void MessageReceived(BMessage *);
virtual bool QuitRequested(void);
//virtual bool QuitRequested(void);

private:

//port_id Port;
BCheckBox *cbox;
BTextControl *btxt;
BColorControl *cctl;
BView *color_sample;
Prefs *prefs;
BLooper *loop;
};

#endif