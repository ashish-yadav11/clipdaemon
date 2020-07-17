/* Copyright (C) 2020 by Ashish Kumar Yadav <ashishkumar.yadav@students.iiserpune.ac.in>
 *
 * forked from ClipIt, which is
 * Copyright (C) 2010-2012 by Cristian Henzel <oss@rspwn.com>
 *
 * ClipIt forked from parcellite, which is
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

static gchar *primary_text;
static gchar *clipboard_text;
static GtkClipboard *primary;
static GtkClipboard *clipboard;

/* Called during the daemon loop to protect primary/clipboard contents */
static void daemon_check()
{
	/* Get current primary/clipboard contents */
	gchar *primary_temp = gtk_clipboard_wait_for_text(primary);
	gchar *clipboard_temp = gtk_clipboard_wait_for_text(clipboard);

	/* Check if primary contents were lost */
	if ((primary_temp == NULL) && (primary_text != NULL)) {
		/* Check contents */
		gint count;
		GdkAtom *targets;
		gboolean contents = gtk_clipboard_wait_for_targets(primary, &targets, &count);

		g_free(targets);
		/* Only recover lost contents if there isn't any other type of content in the clipboard */
		if (!contents)
			gtk_clipboard_set_text(primary, primary_text, -1);
	} else {
		/* Get the button state to check if the mouse button is being held */
		GdkModifierType button_state;
		GdkScreen *screen = gdk_screen_get_default();

		if (screen) {
			GdkDisplay *display = gdk_screen_get_display(screen);
			GdkWindow *window = gdk_screen_get_root_window(screen);
			GdkSeat *seat = gdk_display_get_default_seat(display);

			gdk_window_get_device_position(window, gdk_seat_get_pointer(seat), NULL,
				NULL, &button_state);
		}
		if ((primary_temp != NULL) && !(button_state & GDK_BUTTON1_MASK)) {
			g_free(primary_text);
			primary_text = g_strdup(primary_temp);
		}
	}

	/* Check if clipboard contents were lost */
	if ((clipboard_temp == NULL) && (clipboard_text != NULL)) {
		/* Check contents */
		gint count;
		GdkAtom *targets;
		gboolean contents = gtk_clipboard_wait_for_targets(clipboard, &targets, &count);

		g_free(targets);
		/* Only recover lost contents if there isn't any other type of content in the clipboard */
		if (!contents)
			gtk_clipboard_set_text(clipboard, clipboard_text, -1);
	} else {
		g_free(clipboard_text);
		clipboard_text = g_strdup(clipboard_temp);
	}
	g_free(primary_temp);
	g_free(clipboard_temp);
}

/* Initializes daemon mode */
int main()
{
	/* Initiate GTK+ */
        gtk_init(0, NULL);

	/* Create clipboard and primary and connect signals */
	primary = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

	/* Add the daemon loop */
	/* Register for clipboard change events */
	g_signal_connect(primary, "owner-change", G_CALLBACK(daemon_check), NULL);
	g_signal_connect(clipboard, "owner-change", G_CALLBACK(daemon_check), NULL);

	/* Start daemon loop */
	gtk_main();

        return 0;
}
