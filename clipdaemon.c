/* Copyright (C) 2020 by Ashish Kumar Yadav <ashishkumar.yadav@students.iiserpune.ac.in>
 *
 * forked from ClipIt, which is
 * Copyright (C) 2010-2019 by Cristian Henzel <oss@rspwn.com>
 * Copyright (C) 2011 by Eugene Nikolsky <pluton.od@gmail.com>
 *
 * ClipIt is forked from parcellite, which is
 * Copyright (C) 2007-2008 by Xyhthyx <xyhthyx@gmail.com>
 *
 * This file is part of clipdaemon.
 *
 * clipdaemon is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * clipdaemon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib.h>
#include <glib-object.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gobject/gvaluecollector.h>

gchar *primary_text;
gchar *clipboard_text;
GtkClipboard *primary;
GtkClipboard *clipboard;

void
primary_handler()
{
	/* get text in primary selection */
	gchar *primary_temp = gtk_clipboard_wait_for_text(primary);

	/* check if text in primary selection was lost */
	if (!primary_temp && primary_text) {
		gint count;
		GdkAtom *targets;

		/* only recover primary selection if it doesn't have any other type of content */
		if (!gtk_clipboard_wait_for_targets(primary, &targets, &count))
			gtk_clipboard_set_text(primary, primary_text, -1);
		g_free(targets);
	} else {
		/* get the button state to check if the left mouse button is being held */
		GdkModifierType button_state;
                GdkDisplay *display = gdk_display_get_default();
                GdkScreen *screen = gdk_display_get_default_screen(display);
                GdkSeat *seat = gdk_display_get_default_seat(display);
                GdkWindow *window = gdk_screen_get_root_window(screen);
                GdkDevice *pointer = gdk_seat_get_pointer(seat);

                gdk_window_get_device_position_double(window, pointer, NULL, NULL, &button_state);
		if (primary_temp && !(button_state & GDK_BUTTON1_MASK)) {
			g_free(primary_text);
			primary_text = g_strdup(primary_temp);
		}
	}
	g_free(primary_temp);
}

void
clipboard_handler()
{
	/* get text in clipboard selection */
	gchar *clipboard_temp = gtk_clipboard_wait_for_text(clipboard);

	/* check if text in clipboard selection was lost */
	if (!clipboard_temp && clipboard_text) {
		gint count;
		GdkAtom *targets;

		/* only recover clipboard selection if it doesn't have any other type of content */
		if (!gtk_clipboard_wait_for_targets(clipboard, &targets, &count))
			gtk_clipboard_set_text(clipboard, clipboard_text, -1);
		g_free(targets);
	} else {
		g_free(clipboard_text);
		clipboard_text = g_strdup(clipboard_temp);
	}
	g_free(clipboard_temp);
}

int
main()
{
	/* initiate GTK+ */
	gtk_init(0, NULL);

	/* create primary and clipboard selection objects */
	primary = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

	/* register for clipboard change events */
	g_signal_connect(primary, "owner-change", G_CALLBACK(primary_handler), NULL);
	g_signal_connect(clipboard, "owner-change", G_CALLBACK(clipboard_handler), NULL);

	gtk_main();

	return 0;
}
