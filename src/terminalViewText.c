#include "terminalViewText.h"

GtkTextBuffer* Vt_get_buffer(GtkApplication* app) {
    AppData *data;

    // get buffer from app data
    data = APPDATA(g_object_get_data(G_OBJECT(app), "app_data"));
    if (data == NULL) {
        g_warning("Data not found");
        return NULL;
    }

    if (data->terminalText == NULL) {
        g_warning("Text view not initialized in data");
        return NULL;
    }

    return gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->terminalText));
}

void Vt_print_hello(GtkWidget *widget, gpointer data) {
    (void)widget;
    Vt_append_text(((AppData *)data)->terminal, "Hello world\n");
}

void Vt_append_text(GtkApplication* app, char* text) {
    GtkTextBuffer* textBuffer;
    GtkTextIter iter;

    textBuffer = Vt_get_buffer(app);

    // insert text
    gtk_text_buffer_get_end_iter(textBuffer, &iter);
    gtk_text_buffer_insert(textBuffer, &iter, text, -1);
}

/* void Vt_append_pango_markup(GtkApplication* app, const gchar* markup) {
    GtkTextBuffer* textBuffer;
    GtkTextIter iter;

    textBuffer = Vt_get_buffer(app);


} */