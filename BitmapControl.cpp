#include "BitmapControl.h"
#include <stdio.h>
#include <stdlib.h>


BitmapControl::BitmapControl(BRect frame, BBitmap *On,
		const char *name, const char *label,
		BMessage *message, uint32 resizingMode, uint32 flags):

	BControl(frame, name, label, message, resizingMode, flags),
	my_thread(-1),
	value(0),
	OnPic(On),
	rect(frame),
	mess(message)
{
//	fprintf(stderr, "BitmapControl::BitmapControl called\n");
}



BitmapControl::~BitmapControl()
{
//	fprintf(stderr, "BitmapControl::~BitmapControl called\n");
	if (my_thread > 0) kill_thread(my_thread);
}


void BitmapControl::SetValue(int32 newvalue)
{
	if ((bool)value != (bool)newvalue)
	{
		value = newvalue;

		SetDrawingMode(value ? B_OP_BLEND : B_OP_COPY);
		if (value) SetViewColor(0, 255, 0);
		else SetViewColor(0,0,0);
		Invalidate(); 	//force a redraw

		BControl::SetValue(newvalue);
	}
}

void BitmapControl::Draw(BRect updateRect)
{
	if (OnPic) DrawBitmap(OnPic);
#if 0
	printf("top = %f bottom = %f left = %f right = %f\n",
		updateRect.top,
		updateRect.bottom,
		updateRect.left,
		updateRect.right);
#endif

}

status_t BitmapControl::SetMessage(BMessage *message)
{
	mess = message;
	return BControl::SetMessage(message);
}



/*
 * You slimy bastard!
 *
 *
 * 
 */
void BitmapControl::MouseDown(BPoint point)
{

	if (my_thread > -1) exit(-1);	//shouldn't happen

	my_thread = spawn_thread(ThreadHandler, "BShisen Mouse Thread",
		B_LOW_PRIORITY, this);

	if (my_thread < 0) exit(1);

	resume_thread(my_thread);	//start it up!
}

/*
int32           value;  //cached value;
BBitmap *       OnPic;
BBitmap *       OffPic;
BRect           rect;
BMessage *      mess; 
*/

/*
 * this is a static function which allows threadedness
 *
 *
 */
int32	BitmapControl::ThreadHandler(void * data)
{
BitmapControl *self = (BitmapControl *)data;
thread_id tmp;	

	self->pMouseDown();

	tmp = self->my_thread;

	self->my_thread = -1;

	if (tmp > -1)
		kill_thread(tmp);
	//else fprintf(stderr, "thread terminated prematurely!\n");

	return 1;	//yeah right
}

/*
 * This is in it's own thread....
 *
 * This function tracks the mouse for best responsiveness
 */
void BitmapControl::pMouseDown(void)
{
uint32 buttons;
uint32 _buttons;
BPoint point;

bool was_in;
bool tmp;
int _value = value;


	Window()->Lock();
	GetMouse(&point, &_buttons);	// get the button data initialiiy
	SetValue(!value);
	Window()->Unlock();

/*
 *
 * The problem is, at high speeds, the user may have released
 * before we even get here!
 *
 */
	if (!_buttons)
	{
		Window()->Lock();
		SetValue(!value);
		Window()->Unlock();
		return;
	}
	was_in = true;


	while (1)
	{
		if (!Window()->Lock()) break;	//error!
		GetMouse(&point, &buttons);
		if (!buttons) break;

		tmp = Bounds().Contains(point);
		if (was_in != tmp)
		{
			//something has changed
			was_in = tmp;		//cache it

			SetValue(!value);
		}
		
		Window()->Unlock();
		snooze(20000);
	}

	if (Bounds().Contains(point)) // mouseup _inside_ of the control
	{
		mess->ReplaceInt32("mouse_buttons", _buttons);
		Invoke();	//send a BMessage
	}
	else
	{
		Window()->Lock();
		SetValue(_value);
		Window()->Unlock();

	}

	//when we broke, it was still locked, so unlock it
	Window()->Unlock();
}

