#include "keyHandler.h"

/* gboolean keyval_is_printable_char(guint keyval) {
    return;
} */

void terminal_keypressed(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    (void)widget;

    switch(event->keyval) {
        case GDK_KEY_Tab:

            // Tab

            Vt_append_text(((AppData *)data)->terminal, "<Tab>");
            break;

        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:
            
            // Return - Enter
            Vt_append_text(((AppData *)data)->terminal, "\n");
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(((AppData *)data)->terminalText));
            GtkTextIter iter;
            gtk_text_buffer_get_end_iter(buffer, &iter);

            /* // Move entry
            GtkTextChildAnchor *anchor = gtk_text_buffer_create_child_anchor(buffer, &iter);
            gtk_widget_unparent(((AppData *)data)->terminalEntry);
            gtk_text_view_add_child_at_anchor(GTK_TEXT_VIEW(((AppData *)data)->terminalText), GTK_WIDGET(((AppData *)data)->terminalEntry), anchor); */
            break;

        default:
            char *txt = malloc(50 * sizeof(char));
            snprintf(txt, 50, "%c", gdk_keyval_to_unicode(event->keyval));
            Vt_append_text(((AppData *)data)->terminal, txt);
            free(txt);
            break;
    }
}