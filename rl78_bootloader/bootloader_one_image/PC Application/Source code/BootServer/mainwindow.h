#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void serialRecieved();

    void on_browseButton_clicked();

    void on_parseButton_clicked();

    void on_pushButton_clicked();

    void on_downloadButton_clicked();

    void on_clearButton_clicked();

    void on_selectComButton_clicked();

private:
    Ui::MainWindow *ui;
    void fileTest();
    QByteArray getNextPacketCharacters();
    QByteArray getNextLine();
    uint16_t get_CRC(QByteArray array, uint16_t seed);
    void generate_crc_table();
    void send_array_as_hex(QByteArray array);
    QByteArray get_app_version_from_File();
    QByteArray array_string_to_hex(QByteArray array);
    void reset_file_reader();
    void send_packet();
    void send_data_packet(QByteArray data);
    void send_transfer_complete_packet();
    void send_version_packet();
    void send_reset_packet();
    void send_swap_and_reset_packet();
    QByteArray set_packet_CRC(QByteArray array);
    void clear_serial_in();
    bool newer_version_exists(QByteArray file_version);
    void command_action(QByteArray buffer);
    void resend_packet();
    void update_com_ports();
    void connect_com_port();
};
#endif // MAINWINDOW_H
