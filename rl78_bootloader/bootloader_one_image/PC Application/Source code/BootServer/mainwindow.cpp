#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QFile>
#include <QSerialPort>
#include <QStringList>
#include <QTextStream>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QTextBrowser>
#include <QDebug>

#define REV_POLY        0x8408
#define MAX_PACKETS     0x1FB7      //64 * 0x1FF8 = 7EE00 which is 1 byte past crc

uint16_t CRC_table[256];

QSerialPort *serialPort;
QString serial_in;
QFile appFile;
uint16_t packet_counter = 0;
QByteArray g_rx_buf;
QByteArray packet_data;
QByteArray packet_storage;


uint16_t running_crc = 0;
uint16_t last_data_crc;
uint32_t chunk_counter = 0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    /* Startup initializations */
    ui->setupUi(this);

    /* Generate CRC table */
    generate_crc_table();

    /* Populate the com ports */
    update_com_ports();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(serialPort->isOpen())
        serialPort->close();
    if(appFile.isOpen())
        appFile.close();
}

void MainWindow::serialRecieved()
{
    g_rx_buf.append(serialPort->readAll());

    if(g_rx_buf.length() == 5)
    {
        command_action(g_rx_buf);

        //reset our buffer
        clear_serial_in();
    }
}

void MainWindow::command_action(QByteArray buffer)
{
    QString line;


    if(buffer.at(0) == 0x01)
    {
        /* Reset to bootloader has completed, kickoff download */
        reset_file_reader();

        packet_data = getNextPacketCharacters();

        packet_data = array_string_to_hex(packet_data);

        ui->outputText->append("Beginning Transmission");

        send_data_packet(packet_data);
    }
    else if(buffer.at(0) == 0x02)
    {

        uint16_t receipt_crc;

        receipt_crc = (uint16_t)(buffer[4] & 0xFF) << 8;
        receipt_crc |= buffer[3] & 0xFF;

        if(receipt_crc != last_data_crc)
        {
            QString line;

            line.append("Packet ");
            line.append(QString::number(packet_counter));
            line.append(" Recipt CRC Failed");

            ui->outputText->append(line);

            line.clear();

            line.append("Receipt CRC = ");
            line.append(QString::number(receipt_crc));

            ui->outputText->append(line);

            line.clear();

            line.append("Sent CRC = ");
            line.append(QString::number(last_data_crc));

            ui->outputText->append(line);

            return;
        }

        if(packet_counter < MAX_PACKETS)
        {
            //increment packet counter
            packet_counter++;

            //get next packet and send
            packet_data = getNextPacketCharacters();

            packet_data = array_string_to_hex(packet_data);

            ui->outputText->append("Packet ACK");

            send_data_packet(packet_data);
        }
        else
        {
            ui->outputText->append("Packet ACK");
            ui->outputText->append("Out of packets");
            send_transfer_complete_packet();
        }
    }
    else if(buffer.at(0) == 0x03)
    {
        ui->outputText->append("Packet NAK");
        //Send last packet again
        send_data_packet(packet_data);
    }
    else if(buffer.at(0) == 0x04)
    {
        /* Version has been transmitted verify version# and force reset if needed */

        /* Parse the file for application version */
        QString tmp_string = get_app_version_from_File();
        QByteArray file_version = QByteArray::fromHex(tmp_string.toUtf8());

        buffer = buffer.remove(0, 1); //remove command byte from buffer

        uint32_t board_version_int = 0;
        uint32_t file_version_int = 0;


        file_version_int = (uint32_t) ((uint32_t) file_version[3] << 24);
        file_version_int |= (uint32_t) ((uint32_t) file_version[2] << 16);
        file_version_int |= (uint32_t) ((uint32_t) file_version[1] << 8);
        file_version_int |= (uint32_t) ((uint32_t) file_version[0]);

        board_version_int = (uint32_t) ((uint32_t) buffer[3] << 24);
        board_version_int |= (uint32_t) ((uint32_t) buffer[2] << 16);
        board_version_int |= (uint32_t) ((uint32_t) buffer[1] << 8);
        board_version_int |= (uint32_t) ((uint32_t) buffer[0]);

        if(board_version_int < file_version_int)
        {
            ui->outputText->append("New Version detected.");
            send_swap_and_reset_packet();
        }
        else
        {
            ui->outputText->append("No New Version detected.");
        }
    }
    else if(buffer.at(0) == 0x05)
    {
        //Download SUCCESS! - send s&r to start application
        ui->outputText->append("Download Success!");
        send_swap_and_reset_packet();

        ui->outputText->append("You may now close this program.");

    }
    else if(buffer.at(0) == 0x06)
    {
        //Download FAILED! - restart download
        ui->outputText->append("Download Failed!");
        ui->outputText->append("Attempting download again.");


        QString line;

        line.append("Final Running CRC is : ");
        line.append(QString::number(running_crc));

        ui->outputText->append(line);

        uint16_t receipt_running_crc;

        receipt_running_crc = (uint16_t)(buffer[4] & 0xFF) << 8;
        receipt_running_crc |= buffer[3] & 0xFF;

        if(receipt_running_crc != running_crc)
        {
            ui->outputText->append("Current running CRC does not match calculated one. Please verify packet transmission.");
            return;
        }

        packet_counter = 0;
        reset_file_reader();

        packet_data = getNextPacketCharacters();

        packet_data = array_string_to_hex(packet_data);

        send_data_packet(packet_data);
    }
    else if(buffer.at(0) == 0x07)
    {
        //packet transmission failed, resend packet
        ui->outputText->append("Packet transmission failed.");

        resend_packet();
    }

}

void MainWindow::clear_serial_in()
{
    g_rx_buf.resize(0);
}

void MainWindow::fileTest()
{
    if (!appFile.exists())
    {
        ui->outputText->append("file doesn't exist");
        return;
    }


    if (!appFile.open(QIODevice::ReadWrite | QIODevice::Text))
            return;

    while (!appFile.atEnd()) {
        QByteArray line = appFile.readLine();
        line.chop(20);
        line.remove(0, 10);

        while(line.indexOf(" ") != -1)
        {
            line.remove(line.indexOf(" "), 1);
        }
    }
}

QByteArray MainWindow::getNextPacketCharacters()
{
    QByteArray packet;

    for (int i = 0; i < 4; i++)
    {
        if(!appFile.atEnd())
        {
            QByteArray line = getNextLine();

            if(!line.isNull())
                packet.append(line);
        }
        else
        {
            return NULL;
        }
    }
    return packet;
}

QByteArray MainWindow::getNextLine()
{
    QByteArray line;

    //get line from file
    line = appFile.readLine();

    //remove unnecessary start and end characters
    line.remove(0, 10);
    line.remove(47, line.length());

    //clear spaces
    while(line.indexOf(" ") != -1)
    {
        line.remove(line.indexOf(" "), 1);
    }
    return line;
}

uint16_t MainWindow::get_CRC(QByteArray array, uint16_t seed)
{
    uint16_t crc, i;
    uint8_t  index;

    //Run CRC
    crc = seed;                            /* Set CRCD to Initial Value */

    for(i = 0; i < array.size(); i++)
    {
        index = (uint8_t)(crc ^ array[i]);
        crc = (uint16_t)((crc >> 8) ^ CRC_table[index]);
    }

    return crc;
}

QByteArray MainWindow::set_packet_CRC(QByteArray packet)
{
    uint16_t crc = get_CRC(packet, 0);

    packet[68] = (uint8_t) (crc & 0xFF);
    packet[67] = (uint8_t) ((crc >> 8) & 0xFF);

    return packet;
}

void MainWindow::generate_crc_table()
{
    uint16_t value, temp, i;
    uint8_t  j;

    //Generate Table
    for (i = 0; i < 256; ++i)
    {
        value = 0;
        temp = i;

        //Perform modulo-2 division, a bit at a time
        for (j = 0; j < 8; ++j)
        {
            //Try to divide the current data bit
            if (((value ^ temp) & 0x0001) != 0)
            {
                value = (uint16_t)((value >> 1) ^ REV_POLY);
            }
            else {
                value >>= 1;
            }
            temp >>= 1;
        }
        //Store the result into the table
        CRC_table[i] = value;
    }
}


QByteArray MainWindow::array_string_to_hex(QByteArray array)
{
    //convert array to a standard string
    QString string = array;

    //convert string to hex array
    array = QByteArray::fromHex(string.toUtf8());

    return array;
}

QByteArray MainWindow::get_app_version_from_File()
{
    //Reset the file to read from the first line
    reset_file_reader();

    //The address of the line that the version is on
    QByteArray address("000000D0");

    while (!appFile.atEnd()) {
        QByteArray line = appFile.readLine();

        //If we get to our required line
        if(line.contains(address))
        {
            //chop from end of line
            line.chop(32);

            //remove from begining of line
            line.remove(0, 34);

            while(line.indexOf(" ") != -1)
            {
                line.remove(line.indexOf(" "), 1);
            }

            reset_file_reader();
            return line;
        }
    }
    return NULL;
}

void MainWindow::reset_file_reader()
{
    appFile.close();

    appFile.open(QIODevice::ReadWrite | QIODevice::Text);

    packet_counter = 0;
}

void MainWindow::send_data_packet(QByteArray data)
{
    QByteArray packet;


    packet.resize(3);

    running_crc = get_CRC(data, running_crc);

    //set packet type
    packet[0] = 0x00;

    //apply packet counter
    packet[1] = (uint8_t) (packet_counter & 0xFF);
    packet[2] = (uint8_t) ((packet_counter >> 8) & 0xFF);

    //apply data
    packet.append(data);

    last_data_crc = get_CRC(data, 0);

    uint16_t crc = get_CRC(packet, 0);

    packet.resize(69);

    packet[67] = (uint8_t) (crc & 0xFF);
    packet[68] = (uint8_t) ((crc >> 8) & 0xFF);

    packet_storage = packet;

    QString line;

    line.append("Sending packet: ");
    line.append(QString::number(packet_counter));

    ui->outputText->append(line);

    //write
    serialPort->write(packet);
}

void MainWindow::send_version_packet()
{
    QByteArray packet;

    packet.resize(67);

    //get base structure of packet
    for(int i = 1; i < 67; i++)
    {
        packet[i] = 0xAA;
    }

    //set packet type
    packet[0] = 0x01;

    uint16_t crc = get_CRC(packet, 0);

    packet.resize(69);

    packet[67] = (uint8_t) (crc & 0xFF);
    packet[68] = (uint8_t) ((crc >> 8) & 0xFF);

    //store packet in case of NAK
    packet_storage = packet;

    ui->outputText->append("Sending Version Request");

    //write
    serialPort->write(packet);
}

void MainWindow::send_reset_packet()
{
    QByteArray packet;

    packet.resize(67);

    //get base structure of packet
    for(int i = 1; i < 67; i++)
    {
        packet[i] = 0xAA;
    }

    //set packet type
    packet[0] = 0x02;

    uint16_t crc = get_CRC(packet, 0);

    packet.resize(69);

    packet[67] = (uint8_t) (crc & 0xFF);
    packet[68] = (uint8_t) ((crc >> 8) & 0xFF);

    //store packet in case of NAK
    packet_storage = packet;

    ui->outputText->append("Sending Reset without Swap");

    //write
    serialPort->write(packet);
}

void MainWindow::send_transfer_complete_packet()
{
    QByteArray packet;

    packet.resize(67);

    //get base structure of packet
    for(int i = 1; i < 67; i++)
    {
        packet[i] = 0xAA;
    }

    //set packet type
    packet[0] = 0x03;

    uint16_t crc = get_CRC(packet, 0);

    packet.resize(69);

    packet[67] = (uint8_t) (crc & 0xFF);
    packet[68] = (uint8_t) ((crc >> 8) & 0xFF);

    //store packet in case of NAK
    packet_storage = packet;

    ui->outputText->append("Sending Transfer Complete statement");

    //write
    serialPort->write(packet);
}

void MainWindow::send_swap_and_reset_packet()
{
    QByteArray packet;

    packet.resize(67);

    //get base structure of packet
    for(int i = 1; i < 67; i++)
    {
        packet[i] = 0xAA;
    }

    //set packet type
    packet[0] = 0x04;

    uint16_t crc = get_CRC(packet, 0);

    packet.resize(69);

    packet[67] = (uint8_t) (crc & 0xFF);
    packet[68] = (uint8_t) ((crc >> 8) & 0xFF);

    //store packet in case of NAK
    packet_storage = packet;

    ui->outputText->append("Sending Swap and Reset");

    //write
    serialPort->write(packet);
}

void MainWindow::resend_packet()
{
    QString line;

    line.append("Resending packet: ");
    line.append(QString::number(packet_counter));

    ui->outputText->append(line);
    //write
    serialPort->write(packet_storage);
}



/****************************************************************************************
Source code for UI interface
****************************************************************************************/


void MainWindow::update_com_ports()
{
    ui->comPortBox->clear();

    const auto infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos) {
        ui->comPortBox->addItem(info.portName());
    }
}

void MainWindow::connect_com_port()
{
    serialPort = new QSerialPort(this);

    QString comString;

    comString.append("Selected COM port: ");
    comString.append(ui->comPortBox->currentText());

    ui->outputText->append(comString);

    serialPort->setPortName(ui->comPortBox->currentText().toLower());
    serialPort->setBaudRate(QSerialPort::Baud38400);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->open(QIODevice::ReadWrite);

    connect(serialPort,SIGNAL(readyRead()), this, SLOT(serialRecieved()));
}



void MainWindow::on_browseButton_clicked()
{
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "Hex files (*.hex) ;; All files (*.*)");

    if( !filename.isNull() )
    {
      ui->plainTextEdit->setPlainText(filename.toUtf8());
    }
}

void MainWindow::on_parseButton_clicked()
{
    appFile.close();

    appFile.setFileName(ui->plainTextEdit->toPlainText());

    /* Get file */
    if (!appFile.exists())
    {
        qDebug() << "file doesn't exist";
        return;
    }
    if (!appFile.open(QIODevice::ReadWrite | QIODevice::Text))
            return;


    chunk_counter = 0;

    //Verify files CRC
    while(chunk_counter <= MAX_PACKETS)
    {
        if(chunk_counter == MAX_PACKETS)
        {
            packet_counter++;
        }

        chunk_counter++;
        packet_data = array_string_to_hex(getNextPacketCharacters());

        running_crc = get_CRC(packet_data, running_crc);

    }

    chunk_counter = 0;

    //output crc to textbox
    ui->outputText->append("The file read was successful.");


    if(running_crc == 0)
    {
        ui->outputText->append("File CRC has been verified.");
    }
    else
    {
        ui->outputText->append("File CRC Failed, please check your file and retry.");
    }
}

void MainWindow::on_downloadButton_clicked()
{
    running_crc = 0;
    reset_file_reader();

    appFile.close();

    appFile.setFileName(ui->plainTextEdit->toPlainText());

    /* Get file */
    if (!appFile.exists())
    {
        qDebug() << "file doesn't exist";
        return;
    }
    if (!appFile.open(QIODevice::ReadWrite | QIODevice::Text))
            return;

    clear_serial_in();
    send_version_packet();

}

void MainWindow::on_clearButton_clicked()
{
    ui->outputText->clear();
}

void MainWindow::on_pushButton_clicked()
{
    update_com_ports();
}

void MainWindow::on_selectComButton_clicked()
{
    connect_com_port();
}
