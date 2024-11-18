#include "terminalSetup.h"

void nshTerminal_command_is_sent(GtkWidget* widget, gpointer user_data) {
    AppData* data = APPDATA(user_data);
    (void)widget; (void)data;

    // redirect standar output to a pipe
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        raler("Fatal error : cannot create pipe to redirect exec_command output");
    }

    close(pipefd[0]);
    close(pipefd[1]);
    return;
}

/**
 * Setup the terminal window
*/
void setup_terminal(GtkApplication *app, gpointer user_data) {
    AppData* data = APPDATA(user_data);

    GtkWidget* window;
    
    // main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Nsh");
    gtk_window_set_default_size(GTK_WINDOW(window), 820, 490);

    // window icon
    GdkPixbuf* icon = gdk_pixbuf_new_from_file("nsh192.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    // scrolled window
    data->scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), data->scrolledWindow);

    // text view 
    data->terminalText = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(data->terminalText), FALSE);
    gtk_container_add(GTK_CONTAINER(data->scrolledWindow), data->terminalText);
    gtk_widget_set_can_focus(GTK_WIDGET(data->terminalText), FALSE);

    /* // entry
    data->terminalEntry = gtk_entry_new();
    // insert entry in the text_buffer
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->terminalText));
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);

    GtkTextChildAnchor* anchor = gtk_text_buffer_create_child_anchor(buffer, &iter);
    gtk_text_view_add_child_at_anchor(GTK_TEXT_VIEW(data->terminalText), GTK_WIDGET(data->terminalEntry), anchor); */

    // keyboard handler
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect(G_OBJECT(window), "key_press_event",
                     G_CALLBACK(terminal_keypressed), data);

    // Show all widgets
    gtk_widget_show_all(window);
}

GtkApplication *create_terminal() {
    AppData* data;

    // create app and appdata
    data = g_new0(AppData, 1);
    data->terminal = gtk_application_new(APPID, G_APPLICATION_FLAGS_NONE);
    g_object_set_data_full(G_OBJECT(data->terminal), "app_data", data, g_free);

    // setup ui
    g_signal_connect(data->terminal, "activate", G_CALLBACK(setup_terminal), data);

    return data->terminal;
}
