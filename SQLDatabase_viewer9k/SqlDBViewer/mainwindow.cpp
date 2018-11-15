#include "mainwindow.h"
//#include "im1.xpm"
//#include "im2.xpm"
//#include "im3.xpm"
//#include "im4.xpm"
#define GET_TIME QDateTime::currentDateTime().toString("hh:mm:ss")

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
    //throw_msg(QString("%1: %2\n")),
    menu_bar(new QMenuBar(this)),
    button(new QPushButton("Exec")),
    text_edit(new RETextEdit),
    output_msg(new QTextEdit),
    view(new QTableView),
    model(new QSqlQueryModel(this)),
    output_history_str(""),
    input_history_str (""),
    tool_bar(new QToolBar("Bar"))
{
  MainWindow::setWindowTitle("SQLviewer9000");

  view->setModel(model);

  //this->setStyleSheet("border: 3px solid blue");

  init_menu();

  create_tool_bar();

  construct_window();

  connect(button, &QPushButton::clicked, this, &MainWindow::send_query);
  connect(text_edit, &RETextEdit::entered, this, &MainWindow::send_query);
}

void MainWindow::construct_window()
{
    tool_bar->setStyleSheet("QToolBar {border: 3px solid red}");
    tool_bar->setOrientation(Qt::Vertical);

    output_msg->setReadOnly(true);
    output_msg->setWordWrapMode(QTextOption::NoWrap);
    output_msg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    output_msg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    output_msg->setMaximumHeight(110);
    text_edit->setMaximumHeight(60);

    QHBoxLayout *hlayout_top = new QHBoxLayout;///
    hlayout_top->setMargin(0);
    //hlayout_top->addWidget(menu_bar);

    QGroupBox *view_box_medium = new QGroupBox("Current table: ");
    QHBoxLayout *hlayout_medium = new QHBoxLayout;
    hlayout_medium->setMargin(2);
    //hlayout_medium->addItem(text_block)
    hlayout_medium->addWidget(view);
    view_box_medium->setLayout(hlayout_medium);
    //hlayout_medium->setStretchFactor(0, 3);

    QHBoxLayout *hlayout_bottom = new QHBoxLayout;
    hlayout_bottom->setMargin(2);
    hlayout_bottom->addWidget(new QLabel("Enter a Query:"));
    hlayout_bottom->addWidget(text_edit);
    hlayout_bottom->addWidget(button);

    QGroupBox *view_box_beneath = new QGroupBox("Applicaton output: ");
    view_box_beneath->setMaximumHeight(110);
    QHBoxLayout *hlayout_beneath = new QHBoxLayout;
    hlayout_beneath->setMargin(0);//2
    hlayout_beneath->addWidget(output_msg);
    view_box_beneath->setLayout(hlayout_beneath);

    QVBoxLayout *vlayout_center = new QVBoxLayout;
    //vlayout_center->addLayout(hlayout_top);
    vlayout_center->setMargin(3);
    //vlayout_center->addLayout(hlayout_medium);
    vlayout_center->addWidget(view_box_medium);
    vlayout_center->addLayout(hlayout_bottom);
    //vlayout_center->addWidget(new QLabel("Application Output:"));
    //vlayout_center->addLayout(hlayout_beneath);
    vlayout_center->addWidget(view_box_beneath);


    QHBoxLayout *hlayout_global = new QHBoxLayout;
    hlayout_global->setMargin(5);
    hlayout_global->setMenuBar(tool_bar);
    hlayout_global->addLayout(vlayout_center);

    QVBoxLayout *vlayout_final_wrapper = new QVBoxLayout;
    vlayout_final_wrapper->setMargin(0);
    vlayout_final_wrapper->addWidget(menu_bar);
    ///vlayout_final_wrapper->setMenuBar(tool_bar);
    vlayout_final_wrapper->addLayout(hlayout_global);

//    QBoxLayout *toolLayout (new QBoxLayout(QBoxLayout::TopToBottom, this));
//    toolLayout->setContentsMargins(0, 0, 0, 0);
//    toolLayout->addWidget(tool_bar);
//    toolLayout->setDirection(QBoxLayout::LeftToRight);

            //vlayout_final_wrapper
    this->setLayout(vlayout_final_wrapper);

    //connect(button, &QPushButton::clicked, this, &MainWindow::send_query);///why always type with namespace?
  ///why without ()???????!!!!!!!!!
  ///
    //model->setQuery()
}

void MainWindow::create_tool_bar()
{
    tool_bar->addAction("Create New db");
    tool_bar->addSeparator();
    tool_bar->addAction("Show");
    tool_bar->addSeparator();
    tool_bar->addAction("Help");
    tool_bar->addSeparator();
    tool_bar->addAction("Close db connection");

//    tool_bar->addAction(QPixmap(im1_xpm), "Create New db",this, &MainWindow::ask_database_name);
//    tool_bar->addAction(QPixmap(im2_xpm), "Show",this, &MainWindow::show_db);
//    tool_bar->addAction(QPixmap(im3_xpm), "Help",this, &MainWindow::show_help);
//    tool_bar->addAction(QPixmap(im4_xpm), "Close db connection",this, &MainWindow::ask_database_name);

}

void MainWindow::init_menu()
{
    auto menu_database = menu_bar->addMenu("Database");
    menu_database->addAction("Connect to New Database", this, &MainWindow::ask_database_name, Qt::Key_F5);
    menu_database->addAction("Disconnect", this, &MainWindow::on_disconnection, Qt::Key_F6);
    menu_database->addAction("Show whole DataBase", this, &MainWindow::show_db, Qt::Key_F7);
    menu_database->addSeparator();
    menu_database->addAction("Exit", this, &MainWindow::quit, Qt::Key_F10);//&QCoreApplication::quit()); Why so?


    auto menu_help = menu_bar->addMenu("Help");
    menu_help->addAction("Show Queries", this, &MainWindow::show_help, Qt::Key_F4);

    auto menu_history = menu_bar->addMenu("History");
    menu_history->addAction("Output History", this, &MainWindow::show_output_history, Qt::Key_F11);
    menu_history->addAction("Input History", this, &MainWindow::show_input_history, Qt::Key_F12);

}

void MainWindow::show_output_history()
{
    output_history_edit = new QTextEdit;

    QWidget *new_win1 = new QWidget;
    new_win1->setWindowTitle("History of output");
    output_history_edit->setReadOnly(true);
    output_history_edit->append(output_history_str);
    output_history_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QGroupBox *out_history_groupbox(new QGroupBox("All output notifications: "));
    out_history_groupbox->setAlignment(Qt::AlignHCenter);
    QHBoxLayout *hlayout1(new QHBoxLayout());
    hlayout1->addWidget(output_history_edit);
    out_history_groupbox->setLayout(hlayout1);

    QPushButton *clear_btn1 (new QPushButton("Clear history"));
    QGridLayout *grid1 (new QGridLayout());
    grid1->addWidget(out_history_groupbox, 0, 0);
    grid1->addWidget(clear_btn1, 1, 0);


    new_win1->setLayout(grid1);

    connect(clear_btn1, &QPushButton::clicked,
            output_history_edit, &QTextEdit::clear);

    new_win1->show();
    new_win1->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::show_input_history()
{
    input_history_edit = new QTextEdit;

    QWidget *new_win2 = new QWidget;
    new_win2->setWindowTitle("History of input");
    input_history_edit->setReadOnly(true);
    input_history_edit->append(input_history_str);
    input_history_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QGroupBox *in_history_groupbox(new QGroupBox("All input notifications: "));
    in_history_groupbox->setAlignment(Qt::AlignHCenter);
    QHBoxLayout *hlayout2(new QHBoxLayout());
    hlayout2->addWidget(input_history_edit);
    in_history_groupbox->setLayout(hlayout2);

    QPushButton *clear_btn2 (new QPushButton("Clear history"));
    QGridLayout *grid2 (new QGridLayout());
    grid2->addWidget(in_history_groupbox, 0, 0);
    grid2->addWidget(clear_btn2, 1, 0);

    new_win2->setLayout(grid2);

    connect(clear_btn2, &QPushButton::clicked,
            input_history_edit, &QTextEdit::clear);

    new_win2->show();
    new_win2->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_connection()
{
    text_edit->clear();
    on_disconnection(0);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    db.setDatabaseName(database_name);

    if (db.open())
    {
        set_output_msg (QString("Connection Successful"), "blue");
    }
    else
    {
        set_output_msg("Connection Failure", "red");
        throw_err_window(db);
    }

    exec_output_msg();
    ///output_history_edit->append(throw_msg);
    output_history_str.push_back(throw_msg);

}

void MainWindow::exec_output_msg()
{
    output_msg->append(throw_msg);
    output_msg->verticalScrollBar()->setSliderPosition(
        output_msg->verticalScrollBar()->maximum());
}

void MainWindow::on_disconnection(bool notify = true)
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME, false);

    if(db.isValid() && db.isOpen() )
    {
        db.close();///BOOL METHOD?
        if (notify == true)
        {
            set_output_msg("Disconnection successful", "blue");
            exec_output_msg();
            output_history_edit->append(throw_msg);
        }
    }
    QSqlDatabase::removeDatabase(CONNECTION_NAME);
}

void MainWindow::show_db()
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME, false);

    if (!db.isOpen())
    {
        QMessageBox::critical(this, "Error occured"
                              , QString("Error while connecting to database. Current database state: closed. ")
                              + db.lastError().text());
        set_output_msg("Database is not open", "red");
        output_history_str.push_back(throw_msg);
        exec_output_msg();
        return;
    }

    QString error_str = model->lastError().text().simplified();
    QSqlQuery qry(db);
    qry.prepare("select * from (:1)");
    qry.bindValue(":1", CONNECTION_NAME);
    model->setQuery(qry);

    if (!error_str.isEmpty())
    {
        set_output_msg(error_str, "red");
        output_history_str.push_back(throw_msg);
        exec_output_msg();
    }
}

void MainWindow::send_query()
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME, false);

    QString error_str = model->lastError().text().simplified();
    model->setQuery(text_edit->toPlainText(), db);

    input_history_str.push_back(text_edit->toPlainText() + "\n");
    ///input_history_edit->append(text_edit->toPlainText());

    if (error_str.isEmpty())
    {
        //QString msg = "Querry successfully applied";
        if ((text_edit->toPlainText()).isEmpty())
        {
            set_output_msg("Query is empty", "darkmagenta");
        }
        else
        {
            set_output_msg("Querry successfully applied", "blue");
        }
    }
    else
    {
        set_output_msg(error_str, "red");
    }
    text_edit->clear();
    exec_output_msg();
    output_history_str.push_back(throw_msg);
    ///output_history_edit->append(throw_msg);
}

void MainWindow::show_help()
{
    QWidget *new_win = new QWidget;

    new_win->setWindowTitle("Help on queries");
    QGroupBox *lables_groupbox = new QGroupBox("Queries: ");
    output_help_lables(lables_groupbox);


    QTextEdit *how2qry = new QTextEdit;
    how2qry->setReadOnly(true);


    output_help_instructions(how2qry);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(lables_groupbox);
    layout->addWidget(how2qry);

    new_win->setLayout(layout);

    new_win->show();
    new_win->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::output_help_lables(QGroupBox *lables_groupbox)
{
    QVBoxLayout *lables_layout = new QVBoxLayout;
    lables_layout->setSpacing(20);
    lables_layout->addWidget(new QLabel("Create table:"));///new?how2delthen?
    lables_layout->addWidget(new QLabel("Select all:"));
    lables_groupbox->setLayout(lables_layout);
    lables_groupbox->setAlignment(Qt::AlignLeft);
}

void MainWindow::output_help_instructions(QTextEdit *how2qry)
{
    QString instruction("\nCREATE TABLE addressbook (number INTEGER PRIMARY KEY NOT NULL, "
            "name VARCHAR(15));\n");
    how2qry->append(instruction);
    instruction = "\nSELECT * FROM addressbook\n";
    how2qry->append(instruction);
}


void MainWindow::ask_database_name()
{
    CONNECTION_NAME = QInputDialog::getText(this, "New Database",
                                          "Input a database name: ");
    if (CONNECTION_NAME.isEmpty())
    {
        CONNECTION_NAME = "Untitled";
    }
    database_name = CONNECTION_NAME + QString(".db");
    on_connection();
}

void MainWindow::set_output_msg(QString str, QString color)
{
    throw_msg = "%1: %2\n";
    throw_msg = throw_msg.arg(GET_TIME).arg(str);
    change_text_color(color);
}

void MainWindow::throw_err_window(QSqlDatabase& db)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(this, "Error occured", db.lastError().text(),
                                    QMessageBox::Ok | QMessageBox::Retry);
    if (reply == QMessageBox::Retry)
    {
        on_connection();
    }
}

void MainWindow::change_text_color(QString color)////////////////////!!!!!!!!!
{
//    QString textEditString = text_edit->toHtml();

//     text_edit->clear();
//     text_edit->setHtml(QString("<font color=grey> %1 </font>").arg(textEditString));
    output_msg->setTextColor( QColor( color ) );///red blue black

}

void MainWindow::quit()
{
    on_disconnection();
    QCoreApplication::quit();
}


MainWindow::~MainWindow()
{

}
