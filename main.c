#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <windows.h>

GtkWidget *labels[15];

static gboolean signals_connected = FALSE;
static gboolean signals_connected_remove = FALSE;
static gboolean signals_connected_alternate = FALSE;

static void updateCombobox();

char *argv0;

void restartApplication() {

    LPCSTR applicationPath = argv0;

    HINSTANCE result = ShellExecute(NULL, "open", applicationPath, NULL, NULL, SW_SHOWNORMAL);

    if ((int)result <= 32)
    {
        printf("ShellExecute falhou (%d).\n", (int)result); return;
    }

    exit(0);
}


typedef struct{

    GtkEntry *login;
    GtkEntry *password;
    GtkBuilder *builder;

}Callback;

static void back_window(GtkWidget *widget, GtkBuilder *builder)
{

    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view_inicial");


}

static void consultar_data(GtkBuilder *builder, char* file_Name){

    char* fileName;

    FILE *file = fopen(fileName,"r");
    char buffer[256];
    int line = 0;
    GtkWidget *grid = GTK_WIDGET(gtk_builder_get_object(builder,"grid_consultar"));
    int length = 15;



    if(file != NULL){

        while(fgets(buffer, sizeof(buffer), file))
        {

                sprintf(buffer,"{%s}",buffer);

                labels[line] = gtk_label_new(buffer);

                gtk_grid_attach(GTK_GRID(grid),labels[line],0,line,1,1);//coluna e linha

                line++;

        }

    }

    gtk_widget_show_all(grid);
    fclose(file);

}

static void remover_dados(GtkWidget *widget, GtkBuilder *builder)
{


    GtkEntry *inputMatricula = GTK_ENTRY(gtk_builder_get_object(builder, "entry-matricula-remover"));

    const gchar *txtMatricula = gtk_entry_get_text(inputMatricula);

    FILE *file = fopen("bancoDeDados.txt","r");
    FILE *temp = fopen("temp.txt","w");
    char buffer[1024];
    char bufferTemp[1024];
    int line = 0;
    gboolean found = FALSE;
    int lineAtual = 1;

    while(fgets(buffer, sizeof(buffer), file))
    {
        line++;
        if(strstr(buffer,txtMatricula))
        {
            found = TRUE;
            break;

        }

    }


    if(!found)
    {
        g_print("EQUIPAMENTO N ECONTRADO.");
    }
    else
    {
        rewind(file);
        lineAtual = 1;
        while(fgets(bufferTemp, sizeof(bufferTemp), file))
        {
            if(lineAtual != line)
            {

                fputs(bufferTemp, temp);
            }
            lineAtual++;
        }
    }

    fclose(file);
    fclose(temp);

    remove("bancoDeDados.txt");
    rename("temp.txt","bancoDeDados.txt");

    restartApplication();
}

static void cadastrar_dados(GtkWidget *wideget, GtkBuilder *builder)
{

    //entries
    GtkEntry *inputSetor = GTK_ENTRY(gtk_builder_get_object(builder, "entry-setor"));
    GtkEntry *inputResponsavel = GTK_ENTRY(gtk_builder_get_object(builder, "entry-responsavel-cadastro"));
    GtkEntry *inputMatricula = GTK_ENTRY(gtk_builder_get_object(builder, "entry-matricula-cadastro"));
    GtkEntry *inputNome = GTK_ENTRY(gtk_builder_get_object(builder, "entry-nome-cadastro"));
    GtkComboBoxText *gtkComboUg = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,"button-ug-cadastro"));

    const gchar *txtSetor = gtk_entry_get_text(inputSetor);
    const gchar *txtResponsavel = gtk_entry_get_text(inputResponsavel);
    const gchar *txtMatricula = gtk_entry_get_text(inputMatricula);
    const gchar *txtNome = gtk_entry_get_text(inputNome);

    const char *selected_Ug = gtk_combo_box_text_get_active_text(gtkComboUg);

    char buffer[256];

    sprintf(buffer,"%s %s %s %s %s ",selected_Ug, txtNome, txtMatricula, txtSetor, txtResponsavel);


    FILE *file = fopen("bancoDeDados.txt", "a");

    if (file == NULL) {
        g_print("Erro ao abrir o arquivo!\n");
        return;
    }

    // Escrever o texto no arquivo
    fprintf(file, "%s\n", buffer);

    // Fechar o arquivo
    fclose(file);
    //adicionando elemento no combobox do alterar
    GtkComboBoxText *cmbbtxt = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combobox-item-alterar"));
    gtk_combo_box_text_append(cmbbtxt, NULL, buffer);

    g_print("Dados escritos e arquivo fechado.\n");
}

static void open_cadastrar_window(GtkWidget *widget, GtkBuilder *builder)
{
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder,"stack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view_cadastro");

    if(!signals_connected)
    {

        GtkButton *buttonVoltar = GTK_BUTTON(gtk_builder_get_object(builder, "button-voltar-cadastro"));
        g_signal_connect(buttonVoltar,"clicked", G_CALLBACK(back_window), builder);

        GtkButton *buttonCadastrar = GTK_BUTTON(gtk_builder_get_object(builder, "button-cadastrar-cadastro"));
        g_signal_connect(buttonCadastrar,"clicked", G_CALLBACK(cadastrar_dados), builder);

        signals_connected = TRUE;
    }




}

static void open_remove_window(GtkWidget *widget, GtkBuilder *builder)
{
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder,"stack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view_remover");


    GtkButton *buttonVoltar = GTK_BUTTON(gtk_builder_get_object(builder, "button-voltar-remover"));
    g_signal_connect(buttonVoltar,"clicked", G_CALLBACK(back_window), builder);

    GtkButton *buttonRemover = GTK_BUTTON(gtk_builder_get_object(builder, "button-remover-remover"));
    g_signal_connect(buttonRemover,"clicked", G_CALLBACK(remover_dados), builder);







}

static void alterar_dados(GtkWidget *widget, GtkBuilder *builder)
{
    char buffer[256];
    char bufferTemp[256];
    const char novosDados[256];
    int line = 0;
    gboolean found = FALSE;
    int lineAtual = 1;


    GtkComboBoxText *cmbbtxt = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combobox-item-alterar"));
    const char *selected_text = gtk_combo_box_text_get_active_text(cmbbtxt);

    const char *copy = selected_text;

    const char *Ug = strtok(copy," ");

    //entries
    GtkEntry *inputSetor = GTK_ENTRY(gtk_builder_get_object(builder, "entry-setor-alterar"));
    GtkEntry *inputResponsavel = GTK_ENTRY(gtk_builder_get_object(builder, "entry-responsavel-alterar"));
    GtkEntry *inputCodigo = GTK_ENTRY(gtk_builder_get_object(builder, "entry-codigo-alterar"));
    GtkEntry *inputNome = GTK_ENTRY(gtk_builder_get_object(builder, "entry-nome-alterar"));

    const char *txtSetor = gtk_entry_get_text(inputSetor);
    const char *txtResponsavel = gtk_entry_get_text(inputResponsavel);
    const char *txtCodigo = gtk_entry_get_text(inputCodigo);
    const char *txtNome = gtk_entry_get_text(inputNome);


    if (!inputSetor)
    {
            g_print("Erro ao obter entry-setor-alterar.\n"); return;
    }else{
        g_print("funcionou\n");
    }


    if(inputCodigo == NULL)
    {
       g_print("vtf");
    }



    if (builder == NULL) {
        g_print("Erro ao obter o GtkComboBoxText.\n");
        return;
    }

    //encontrar a linha do item selecionado

    FILE *file = fopen("bancoDeDados.txt","r");
    FILE *temp = fopen("temp.txt","w");

    if(file == NULL || temp == NULL)
    {
        fclose(file);
        fclose(temp);
        return;
    }

    while(fgets(buffer, sizeof(buffer), file))
    {
        line++;
        if(strstr(buffer,selected_text))
        {
            found = TRUE;
            break;

        }
    }

    if(!found)
    {
        g_print("item nao selecionado");
    }
    else
    {
        rewind(file);
        lineAtual = 1;
        while(fgets(bufferTemp, sizeof(bufferTemp), file))
        {
            if(lineAtual == line)
            {
                snprintf(novosDados, sizeof(novosDados), "%s %s %s %s %s\n", Ug, txtNome, txtCodigo, txtSetor, txtResponsavel);

                fputs(novosDados, temp);
            }
            else
            {
                fputs(bufferTemp, temp);
            }
            lineAtual++;
        }



    }

    fclose(file);
    fclose(temp);

    remove("bancoDeDados.txt");
    rename("temp.txt","bancoDeDados.txt");

    restartApplication();

}

static void updateCombobox(GtkBuilder *builder)
{
    GtkComboBoxText *cmbbtxt = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combobox-item-alterar"));


    FILE *file = fopen("BancoDeDados.txt","r");
    char buffer[1024];

    while(fgets(buffer, sizeof(buffer), file))
    {
        gtk_combo_box_text_append(cmbbtxt, NULL, buffer);

    }

    fclose(file);

}

static void open_alterar_window(GtkWidget *widget, GtkBuilder *builder)
{
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder,"stack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view_alterar");
    if(!signals_connected_alternate)
    {
        GtkButton *buttonVoltar = GTK_BUTTON(gtk_builder_get_object(builder, "button-voltar-alterar"));
        g_signal_connect(buttonVoltar,"clicked", G_CALLBACK(back_window), builder);

        GtkButton *buttonAlterar = GTK_BUTTON(gtk_builder_get_object(builder, "button-alterar-alterar"));
        g_signal_connect(buttonAlterar,"clicked", G_CALLBACK(alterar_dados), builder);
        signals_connected_alternate = TRUE;
    }







}


static void open_consultar_window(GtkWidget *widget, GtkBuilder *builder)
{

    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder,"stack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view_consultar");

    GtkButton *buttonVoltar = GTK_BUTTON(gtk_builder_get_object(builder, "button-voltar-consultar"));
    g_signal_connect(buttonVoltar,"clicked", G_CALLBACK(back_window), builder);

    consultar_data(builder,"bancoDeDados");


}

static void open_home_screen(GtkBuilder *builder)
{
    //abrindo tela inicial
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view_inicial");

    //abrindo telas
    GtkButton *buttonCadastrar = GTK_BUTTON(gtk_builder_get_object(builder, "button_cadastrar_inicial"));
    g_signal_connect(buttonCadastrar,"clicked",G_CALLBACK(open_cadastrar_window), builder);

    GtkButton *buttonRemove = GTK_BUTTON(gtk_builder_get_object(builder, "button_remover_inicial"));
    g_signal_connect(buttonRemove,"clicked",G_CALLBACK(open_remove_window), builder);

    GtkButton *buttonAlterar = GTK_BUTTON(gtk_builder_get_object(builder, "button_altera_inicial"));
    g_signal_connect(buttonAlterar,"clicked",G_CALLBACK(open_alterar_window), builder);

    GtkButton *buttonConsultar = GTK_BUTTON(gtk_builder_get_object(builder, "button_consultar_inicial"));
    g_signal_connect(buttonConsultar,"clicked",G_CALLBACK(open_consultar_window), builder);

    GtkButton *buttonQuit = GTK_BUTTON(gtk_builder_get_object(builder, "button_sair_inicial"));
    g_signal_connect(buttonQuit,"clicked",G_CALLBACK(gtk_main_quit), NULL);

}


//functions system

static void verify_data_user(gchar *login, gchar *password, GtkBuilder *builder)
{
    FILE *file = fopen("dados.txt","r");
    gchar dados[30];
    gchar *loginAndPassword[2];
    gchar ch;
    int i = 0, j = 0;

    while((ch = getc(file)) != EOF)
    {
        dados[i] = ch;
        i++;

    }dados[i] = '\0';

    gchar *token = strtok(dados, " ");

    while (token != NULL)
    {
        loginAndPassword[j] = token;
        j++;
        token = strtok(NULL, " ");

    }


    //verify
    if(strcmp(loginAndPassword[0], login) == 0 && strcmp(loginAndPassword[1], password) == 0)
    {

        open_home_screen(builder);
    }
    else{
        g_print("ERROU!!!!");
    }

}


static void on_button_login_clicked(GtkWidget *button, gpointer user_data)
{
    //cast
    Callback *data = (Callback *)user_data;
    //get
    const gchar *login = gtk_entry_get_text(data->login);
    const gchar *password = gtk_entry_get_text(data->password);
    const GtkBuilder *builder = data->builder;

    //verify user
    verify_data_user(login, password, builder);


}





int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    GtkBuilder *builder;
    GtkWidget *window;

    argv0 = argv[0];

    //create builder
    builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, "projeto_hemir.glade", NULL))
    {
        g_error("Falha ao carregar o arquivo .glade!");
        return 1;
    }

    updateCombobox(builder);

    //signals
    GtkButton  *buttonLogin = GTK_BUTTON(gtk_builder_get_object(builder,"button-login"));
    GtkEntry *entryLogin = GTK_ENTRY(gtk_builder_get_object(builder, "matricula"));
    GtkEntry *entryPassword = GTK_ENTRY(gtk_builder_get_object(builder, "senha"));

    Callback entradas = {entryLogin, entryPassword, builder};

    g_signal_connect(buttonLogin, "clicked", G_CALLBACK(on_button_login_clicked), &entradas);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //build window
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    gtk_widget_show_all(window);

    gtk_main();
    g_object_unref(builder);






    return 0;

}


