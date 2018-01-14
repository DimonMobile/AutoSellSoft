#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::_updateAmounts()
{
    QString textForDisplay;

    textForDisplay.append(QString("Количество клиентов: %1\n").arg(_clientCards.size()));
    //checking for clients is empty condition with tern operator
    (_clientCards.isEmpty())
    ? textForDisplay.append(QString("Доступный промежуток: %1 - %2").arg(0).arg(0))
    : textForDisplay.append(QString("Доступный промежуток: %1 - %2").arg(0).arg(_clientCards.size()-1));
    //----------------------------------------------------------
    ui->spinBox->setMinimum(0);
    ui->spinBox->setMaximum(_clientCards.size()-1);

    ui->amounts_lbl->setText(textForDisplay);
    ui->amounts_lbl->adjustSize();
}

void MainWindow::_displayClientInfo(const int index)
{
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    int prevHeight(0);

    QGridLayout * customGridLayout = new QGridLayout;
    QWidget * widgetContainer = new QWidget;
    widgetContainer->setLayout(customGridLayout);
    ui->detailData_scrollArea->setWidget(widgetContainer);

    _clientCards[index].servicesEditList.clear();
    _clientCards[index].phonesEditList.clear();
    _clientCards[index].emailEditList.clear();
    qDebug() << "Edits cleared";
    //ID
    _clientCards[index].idLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].idLbl->setText( QString("Id: %1").arg( _clientCards[index].getId() ) );
    _clientCards[index].idLbl->show();
    customGridLayout->addWidget(_clientCards[index].idLbl, prevHeight, 0);
    //reseter
    _clientCards[index].resetFunnelBtn = new QPushButton(ui->detailData_scrollArea);
    _clientCards[index].resetFunnelBtn->setText("Сброс воронки");
    if (_clientCards[index].getBlockId() == -1)
        _clientCards[index].resetFunnelBtn->setEnabled(false);
    connect(_clientCards[index].resetFunnelBtn, SIGNAL(clicked(bool)), SLOT(onFunnelResetBtnClicked(bool)));
    customGridLayout->addWidget(_clientCards[index].resetFunnelBtn, prevHeight, 1);
    ++prevHeight;
    //Названице
    _clientCards[index].displayLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].displayLbl->setText(QString("%1").arg(_clientCards[index].getDisplayText()));
    _clientCards[index].displayLbl->show();
    customGridLayout->addWidget(_clientCards[index].displayLbl, prevHeight, 1);
    ++prevHeight;
    //Название организации лейбл
    _clientCards[index].organizationNameLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].organizationNameLbl->setText(QString("Название организации:"));
    _clientCards[index].organizationNameLbl->show();
    customGridLayout->addWidget(_clientCards[index].organizationNameLbl, prevHeight, 0, Qt::AlignRight);
    //Название организации едит сам великий
    _clientCards[index].organizationNameEdit = new QLineEdit(ui->detailData_scrollArea);
    _clientCards[index].organizationNameEdit->setText(_clientCards[index].getOrganizationName());
    _clientCards[index].organizationNameEdit->show();
    customGridLayout->addWidget(_clientCards[index].organizationNameEdit, prevHeight, 1);
    ++prevHeight;
    //Контактное лицо label
    _clientCards[index].contactFaceLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].contactFaceLbl->setText("Контактное лицо:");
    _clientCards[index].contactFaceLbl->show();
    customGridLayout->addWidget(_clientCards[index].contactFaceLbl, prevHeight, 0, Qt::AlignRight);
    //Контактное лицо label
    _clientCards[index].contactFaceEdit = new QLineEdit(ui->detailData_scrollArea);
    _clientCards[index].contactFaceEdit->setText(_clientCards[index].getContactFace());
    _clientCards[index].contactFaceEdit->show();
    customGridLayout->addWidget(_clientCards[index].contactFaceEdit, prevHeight, 1);
    ++prevHeight;
    //По файликам строчка
    _clientCards[index].funnelFileNameLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].funnelFileNameLbl->setText("Путь к файлу воронки:");
    customGridLayout->addWidget(_clientCards[index].funnelFileNameLbl, prevHeight, 0, Qt::AlignRight);
    ++prevHeight;
    //Едит со строкой
    _clientCards[index].funnelFileNameEdt = new QLineEdit(ui->detailData_scrollArea);
    _clientCards[index].funnelFileNameEdt->setText(_clientCards[index].getFunnelFileName());
    _clientCards[index].funnelFileNameEdt->setReadOnly(true);
    _clientCards[index].funnelFileNameEdt->setPlaceholderText("Путь к файлу воронки");
    customGridLayout->addWidget(_clientCards[index].funnelFileNameEdt, prevHeight, 1);
    //Кнопочка к нему
    _clientCards[index].funnelFileNameTBtn = new QToolButton(ui->detailData_scrollArea);
    _clientCards[index].funnelFileNameTBtn->setText("...");
    if (_clientCards[index].getBlockId() == -1)
        _clientCards[index].funnelFileNameTBtn->setEnabled(true);
    else
        _clientCards[index].funnelFileNameTBtn->setEnabled(false);
    connect(_clientCards[index].funnelFileNameTBtn, SIGNAL(clicked(bool)), SLOT(onFunnelFileNameBrowseBtnClicked(bool)));
    customGridLayout->addWidget(_clientCards[index].funnelFileNameTBtn, prevHeight, 0, Qt::AlignRight);

    ++prevHeight;
    //
    _clientCards[index].servicesLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].servicesLbl->setText("Наличие клиента в сервисах:");
    _clientCards[index].servicesLbl->show();
    customGridLayout->addWidget(_clientCards[index].servicesLbl, prevHeight, 0, Qt::AlignRight);
    ++prevHeight;
    //services edits list
    {
        QVector<QPair<QString, qint64> > serviceList = _clientCards[index].getServiceList();
        for(QVector<QPair<QString, qint64> >::const_iterator i = serviceList.begin(); i != serviceList.end(); ++i)
        {
            QLineEdit * lnEdt1 = new QLineEdit(ui->detailData_scrollArea);
            QLineEdit * lnEdt2 = new QLineEdit(ui->detailData_scrollArea);
            lnEdt1->setPlaceholderText("Сервис");
            lnEdt2->setPlaceholderText("ID");

            lnEdt1->setText(i->first);
            lnEdt2->setText(QString::number(i->second));

            lnEdt1->show();
            lnEdt2->show();
            _clientCards[index].servicesEditList << QPair<QLineEdit*, QLineEdit*>(lnEdt1, lnEdt2);
            customGridLayout->addWidget(lnEdt1, prevHeight, 0);
            customGridLayout->addWidget(lnEdt2, prevHeight, 1);
            ++prevHeight;
        }
        QToolButton * btn = new QToolButton(ui->detailData_scrollArea);
        connect(btn, SIGNAL(clicked(bool)), SLOT(onAddServiceFieldsBtnClicked()));
        btn->setText("+");
        btn->show();
        customGridLayout->addWidget(btn, prevHeight, 0);
        ++prevHeight;
    }
    //
    _clientCards[index].phonesLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].phonesLbl->setText("Телефонные номера клиента:");
    _clientCards[index].phonesLbl->show();
    customGridLayout->addWidget(_clientCards[index].phonesLbl, prevHeight, 0, Qt::AlignRight);
    ++prevHeight;
    //phones edits list
    {
        QVector<QPair<QString, QString> > phoneList = _clientCards[index].getPhoneList();
        for(QVector<QPair<QString, QString> >::const_iterator i = phoneList.begin(); i != phoneList.end(); ++i)
        {
            QLineEdit * lnEdt1 = new QLineEdit(ui->detailData_scrollArea);
            QLineEdit * lnEdt2 = new QLineEdit(ui->detailData_scrollArea);
            lnEdt1->setPlaceholderText("Сервис");
            lnEdt2->setPlaceholderText("Номер телефона");

            lnEdt1->setText(i->first);
            lnEdt2->setText(i->second);

            lnEdt1->show();
            lnEdt2->show();
            _clientCards[index].phonesEditList << QPair<QLineEdit*, QLineEdit*>(lnEdt1, lnEdt2);
            customGridLayout->addWidget(lnEdt1, prevHeight, 0);
            customGridLayout->addWidget(lnEdt2, prevHeight, 1);
            ++prevHeight;
        }
        QToolButton * btn = new QToolButton(ui->detailData_scrollArea);
        connect(btn, SIGNAL(clicked(bool)), SLOT(onAddPhoneFieldsBtnClicked()));
        btn->setText("+");
        btn->show();
        customGridLayout->addWidget(btn, prevHeight, 0);
        ++prevHeight;
    }
    //email label
    _clientCards[index].emailsLbl = new QLabel(ui->detailData_scrollArea);
    _clientCards[index].emailsLbl->setText("Электронные почты клиента:");
    _clientCards[index].emailsLbl->show();
    customGridLayout->addWidget(_clientCards[index].emailsLbl, prevHeight, 0, Qt::AlignRight);
    ++prevHeight;
    //email edits list
    {
        QVector<QPair<QString, QString> > emailList = _clientCards[index].getEmailList();
        for(QVector<QPair<QString, QString> >::const_iterator i = emailList.begin(); i != emailList.end(); ++i)
        {
            QLineEdit * lnEdt1 = new QLineEdit(ui->detailData_scrollArea);
            QLineEdit * lnEdt2 = new QLineEdit(ui->detailData_scrollArea);
            lnEdt1->setPlaceholderText("Сервис");
            lnEdt2->setPlaceholderText("Email адрес");

            lnEdt1->setText(i->first);
            lnEdt2->setText(i->second);

            lnEdt1->show();
            lnEdt2->show();
            _clientCards[index].emailEditList << QPair<QLineEdit*, QLineEdit*>(lnEdt1, lnEdt2);
            customGridLayout->addWidget(lnEdt1, prevHeight, 0);
            customGridLayout->addWidget(lnEdt2, prevHeight, 1);
            ++prevHeight;
        }
        QToolButton * btn = new QToolButton(ui->detailData_scrollArea);
        connect(btn, SIGNAL(clicked(bool)), SLOT(onAddEmailFieldsBtnClicked()));
        btn->setText("+");
        btn->show();
        customGridLayout->addWidget(btn, prevHeight, 0);
        ++prevHeight;
    }
    {
        QPushButton * btn = new QPushButton(ui->detailData_scrollArea);
        btn->setText("Применить");
        connect(btn, SIGNAL(clicked()), SLOT(onApplyBtnClicked()));
        customGridLayout->addWidget(btn, prevHeight, 1, Qt::AlignRight);
        btn->show();
        ++prevHeight;
    }
}

void MainWindow::_fromEditsToCard(const int index)
{
    ClientCard &currentCard = _clientCards[ui->spinBox->value()];
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    currentCard.setOrganizationName(currentCard.organizationNameEdit->text());
    currentCard.setContactFace(currentCard.contactFaceEdit->text());
    currentCard.setFunnelFileName(currentCard.funnelFileNameEdt->text());
    currentCard.loadServiceFromEdits();
    currentCard.loadPhoneFromEdits();
    currentCard.loadEmailFromEdits();
}

void MainWindow::_saveData()
{
    QFile file("clients.dat");
    try
    {
        if (!file.open(QIODevice::WriteOnly))
            throw QString("Ошибка сохранения данных (ошибка открытия файла)");
        QDataStream dataStream(&file);
        dataStream.setVersion(QDataStream::Qt_5_9);
        dataStream << _clientCards.size();
        for(QList<ClientCard>::const_iterator i = _clientCards.begin(); i != _clientCards.end(); ++i)
        {
            dataStream << i->getId();
            dataStream << i->getLastLinkId();
            dataStream << i->getBlockExecDateTime();
            dataStream << i->getBlockId();
            dataStream << i->getDisplayText();
            dataStream << i->getOrganizationName();
            dataStream << i->getContactFace();
            dataStream << i->getFunnelFileName();
            dataStream << i->getServiceList();
            dataStream << i->getPhoneList();
            dataStream << i->getEmailList();
            if (dataStream.status() != QDataStream::Ok)
                throw QString("Ошибка записи данных");
        }
        //QMessageBox::information(this, "Успех", "Данные успешно сохранены");
    }
    catch(QString &e)
    {
        QMessageBox::critical(this, "Error", e);
    }
    file.close();
}

void MainWindow::_loadData()
{
    ui->spinBox->setMinimum(-1);
    ui->spinBox->setValue(-1);
    QFile file("clients.dat");
    try
    {
        if (!file.open(QIODevice::ReadOnly))
            throw QString("Ошибка загрузки данных (ошибка открытия файла)");
        QDataStream dataStream(&file);
        dataStream.setVersion(QDataStream::Qt_5_9);
        int structAmount;
        dataStream >> structAmount;
        _clientCards.clear();
        for(int i = 0 ; i < structAmount; ++i)
        {
            ClientCard card;
            qint32 id;//
            quint32 lastLinkId;
            QDateTime blockExecTime;
            qint32 blockId;
            QString displayText;
            QString organizationName;
            QString contactFace;
            QString funnelFileName;
            QVector<QPair<QString, qint64> > serviceList;
            QVector<QPair<QString, QString> > phoneList;
            QVector<QPair<QString, QString> > emailList;
            //reading data
            dataStream >> id;
            dataStream >> lastLinkId;
            dataStream >> blockExecTime;
            dataStream >> blockId;
            dataStream >> displayText;
            dataStream >> organizationName;
            dataStream >> contactFace;
            dataStream >> funnelFileName;
            dataStream >> serviceList;
            dataStream >> phoneList;
            dataStream >> emailList;
            //copying data
            card.setId(id);
            card.setLastLinkId(lastLinkId);
            card.setBlockExecDateTime(blockExecTime);
            card.setBlockId(blockId);
            card.setDisplayText(displayText);
            card.setOrganizationName(organizationName);
            card.setContactFace(contactFace);
            card.setFunnelFileName(funnelFileName);
            card.setServiceList(serviceList);
            card.setPhoneList(phoneList);
            card.setEmailList(emailList);
            _clientCards.append(card);
            if (dataStream.status() != QDataStream::Ok)
                throw QString("Ошибка чтения данных");
        }
        _updateAmounts();
    }
    catch(QString &e)
    {
        QMessageBox::critical(this, "Error", e);
    }
    file.close();
}

void MainWindow::_addLog(const QString &color, const QString &text)
{
    ui->textEdit->moveCursor(QTextCursor::Down);
    ui->textEdit->insertHtml(tr("<font color=%1>%2</font><br>").arg(color).arg(text));
    QApplication::processEvents();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _updateAmounts();
    _intervalTimer = new QTimer(this);
    _vkProcessor = new VkProcessor(this);
    connect(_intervalTimer, SIGNAL(timeout()), SLOT(onIntervalTimerTimeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{
    bool isOk;
    QString str = QInputDialog::getText(this, "Название", "Придумайте название для клиента", QLineEdit::Normal, "<новый клиент>", &isOk);
    if (isOk)
        _clientCards.append(ClientCard(_clientCards.size(), str));
    _updateAmounts();
    ui->spinBox->setValue(_clientCards.size()-1);
}

void MainWindow::onAddServiceFieldsBtnClicked()
{
    int index = ui->spinBox->value();
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    _fromEditsToCard(index);
    _clientCards[ui->spinBox->value()].addEmptyServiceNode();
    _displayClientInfo(ui->spinBox->value());
}

void MainWindow::onAddPhoneFieldsBtnClicked()
{
    int index = ui->spinBox->value();
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    _fromEditsToCard(index);
    _clientCards[ui->spinBox->value()].addEmptyPhoneNode();
    _displayClientInfo(ui->spinBox->value());
}

void MainWindow::onAddEmailFieldsBtnClicked()
{
    int index = ui->spinBox->value();
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    _fromEditsToCard(index);
    _clientCards[ui->spinBox->value()].addEmptyEmailNode();
    _displayClientInfo(ui->spinBox->value());
}

void MainWindow::onApplyBtnClicked()
{
    int index = ui->spinBox->value();
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    _fromEditsToCard(index);
    _displayClientInfo(index);
}

void MainWindow::onFunnelFileNameBrowseBtnClicked(bool checked)
{
    Q_UNUSED(checked);
    int index = ui->spinBox->value();
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    QString string = QFileDialog::getOpenFileName(this, tr("Укажите путь к файлу"), "", "Funnel table resource (*.ftr)");
    if (string.isEmpty()) return;
    _clientCards[index].funnelFileNameEdt->setText(string);
}

void MainWindow::onFunnelResetBtnClicked(bool checked)
{
    Q_UNUSED(checked);
    int index = ui->spinBox->value();
    if ( (index > _clientCards.size()-1) ||  index < 0) return;
    _clientCards[index].setBlockId(-1);
    _displayClientInfo(index);
}

void MainWindow::onIntervalTimerTimeout()
{
    --_intervalTimerAmount;
    if (_intervalTimerAmount == 0)
    {
        _intervalTimer->stop();
        on_pushButton_clicked();
        if (ui->checkBox->isChecked())
            _saveData();
        if (ui->pushButton_2->isChecked())
        {
            on_pushButton_2_toggled(true);
        }
    }
    ui->lcdNumber->display(_intervalTimerAmount);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    _displayClientInfo(arg1);
}

void MainWindow::on_action_triggered()
{
    _loadData();
}

void MainWindow::on_action_2_triggered()
{
    _saveData();
}

void MainWindow::on_action_SMTP_triggered()
{
    MailAccountsDialog * mailDialog = new MailAccountsDialog(this);
    mailDialog->exec();
    delete mailDialog;
}

void MainWindow::on_action_3_triggered()
{
    TreeEdit *treeEdit = new TreeEdit(this);
    treeEdit->exec();
    delete treeEdit;
}

void MainWindow::on_action_MySQL_triggered()
{
    DataBaseDialog * dbDialog = new DataBaseDialog(this);
    if (dbDialog->exec() == QDialog::Accepted)
    {
        DataBaseSettings dbSet = dbDialog->getSettings();
        dbSet.saveSettings();
        qDebug() << "Settings saved";
    }
    delete dbDialog;
}

void MainWindow::on_action_4_triggered()
{
    LinkGenerator * gen = new LinkGenerator(this);
    gen->exec();
    delete gen;
}

void MainWindow::on_pushButton_clicked()
{
    ui->status_lbl->setText(tr("Начинаем"));
    ui->textEdit->clear();
    _addLog("blck", tr("Начало. Для обработки: %1").arg(_clientCards.size()));
    QVector<QString> errorMessages;
    errorMessages.resize(_clientCards.size());
    for(int i = 0; i < _clientCards.size(); ++i)
    {
        ui->status_lbl->setText(tr("Обработка %1/%2").arg(i+1).arg(_clientCards.size()));
        _addLog("blue", tr("***Клиент %1").arg(i+1));
        TreeEdit treeEdit(this);
        treeEdit.loadFromFile(_clientCards[i].getFunnelFileName());
        GraphicsBlock * block = treeEdit.getCurrentBlock(_clientCards[i].getBlockId());
        qDebug() << "CurrentBlockText:" << block->text();
        if (block == nullptr)
        {
            //Тут вообще какая-то не внятная ошибка если че потом выведем ее
            _addLog("red", tr("Тут ошибочка с указателями"));
            qCritical() << "Ну тут ошибочка с указателями какая-то";
            continue;
        }
        //Выполняем шмотье
        switch(block->blockType())
        {
        case TypeActionBegin:
        {
            GraphicsBlock *nextBlock = treeEdit.nextPositiveBlock(block);
            if (nextBlock == nullptr)
            {
                errorMessages[i] = treeEdit.getErrorString();
                break;
            }
            qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
            _clientCards[i].setBlockId(nextIdx);
            _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
            _addLog("green", "Завершен начальный блок");
            break;
        }
        case TypeActionSendVK:
        {
            qDebug() << "Появились в блоке отправки помойки";
            GraphicsBlock *nextBlock = treeEdit.nextPositiveBlock(block);
            if (nextBlock == nullptr)
            {
                errorMessages[i] = treeEdit.getErrorString();
                break;
            }
            qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
            bool isFound;
            qint64 vkId = _clientCards[i].getServiceId("VK", &isFound);
            QString messageForSend(block->content());
            messageForSend.replace("{CLIENT_NAME}", _clientCards[i].getContactFace());
            if (!_vkProcessor->sendMessage(vkId, messageForSend))
            {
                errorMessages[i] = tr("Ошибка отправки сообщения VK к %1").arg(vkId);
                break;
            }
            //Переход на следующий
            _clientCards[i].setBlockId(nextIdx);
            _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
            _addLog("green", "Завершен блок отправки сообщения VK");
            break;
        }
        case TypeActionSendEmail:
        {
            GraphicsBlock *nextBlock = treeEdit.nextPositiveBlock(block);
            if (nextBlock == nullptr)
            {
                errorMessages[i] = treeEdit.getErrorString();
                break;
            }
            qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);

            SMTPSettings smSettings(this);
            smSettings.load();

            auto emailList = _clientCards[i].getEmailList();
            if (emailList.isEmpty())
            {
                errorMessages[i] = tr("Ошибка отправки Email (у клиента нет аккаунтов)");
                break;
            }
            QString currentClientEmail(emailList.at(rand() %emailList.size()).second);
            QString currentEmailString(block->content());
            currentEmailString.replace("{CLIENT_NAME}", _clientCards[i].getContactFace());
            currentEmailString.replace("{SENDER_MAIL}", smSettings.getEmail());

            QRegExp regExp("\\{GENERATE_LINK\\(\\\"(.+)\\\"\\,\\\"(.+)\\\"\\)\\}");
            int pos = regExp.indexIn(currentEmailString);
            if (pos != -1)
            {
                QString readyLink;
                quint32 linkId;
                QString domain = regExp.cap(1);
                QString redirrectUrl = regExp.cap(2);
                if (!LinkGenerator::generateLink(i, domain, redirrectUrl, nullptr, &readyLink, &linkId))
                {
                    errorMessages[i] = tr("Ошибка генерации ссылки");
                    break;
                }
                //Заменим в сообщении
                currentEmailString.replace(regExp, readyLink);
                //Сохраним ID клиенту
                _clientCards[i].setLastLinkId(linkId);
            }
            if (!MailAccountsDialog::sendMail(smSettings.getHost(), smSettings.getPort(), smSettings.getEmail(), currentClientEmail, smSettings.getAuthorize(), smSettings.getUseSsl(), smSettings.getUsername(), smSettings.getPassword(), currentEmailString))
            {
                errorMessages[i] = tr("Ошибка отправки Email(содержание/соединение)");
                break;
            }
            _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
            _clientCards[i].setBlockId(nextIdx);//Переход на следущий
            _addLog("green", "Успешная отправка Email");
            break;
        }
        case TypeActionWait:
        {
            qDebug() << "Зашли в блок ожидания";
            GraphicsBlock *nextBlock = treeEdit.nextPositiveBlock(block);
            if (nextBlock == nullptr)
            {
                errorMessages[i] = treeEdit.getErrorString();
                break;
            }
            qint64 needTime = block->content().toLongLong() * 60 * 60;
            qint64 elTime = _clientCards[i].getBlockExecDateTime().secsTo(QDateTime::currentDateTime());
            if (elTime > needTime)
            {
                qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
                _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
                _clientCards[i].setBlockId(nextIdx);//Переход на следущий
                _addLog("green", "Блок ожидание пройден успешно");
                qDebug() << "Ожидание прошло, ушли на след. блок";
            }
            else
            {
                qDebug() << "Блок еще не готов к переходу, осталось" << needTime - elTime << "секунд";
                _addLog("orange", tr("Блок ожидания не пропускает, осталось %1 сек.").arg(needTime - elTime));
            }
            break;
        }
        case TypeActionAsk:
        {
            qDebug() << "Зашли в блок вопроса оператору";
            int resD;
            if((resD = QMessageBox::question(this, "Вопрос", block->content(), QMessageBox::Yes, QMessageBox::No, QMessageBox::Ignore)) == QMessageBox::Yes)
            {
                GraphicsBlock *nextBlock = treeEdit.nextPositiveBlock(block);
                qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
                _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
                _clientCards[i].setBlockId(nextIdx);//Переход на следущий
                _addLog("green", "На вопрос ответили положительно");
            }
            else if (resD == QMessageBox::No)
            {
                GraphicsBlock *nextBlock = treeEdit.nextNegativeBlock(block);
                qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
                _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
                _clientCards[i].setBlockId(nextIdx);//Переход на следущий
                _addLog("green", "На вопрос ответили отрицательно");
            }
            else
            {
                qDebug() << "Оператор пропустил вопрос, он будет задан снова";
                _addLog("orange", tr("Оператор пропустил вопрос, он будет задан снова"));
            }
            break;
        }
        case TypeActionLinkCheck:
        {
            QNetworkAccessManager nManager(this);
            QUrlQuery uQuery;
            DataBaseSettings settings(this);
            settings.loadSettings();
            uQuery.addQueryItem("access_hash", settings.webInterfaceKey());
            uQuery.addQueryItem("link_id", QString::number(_clientCards[i].getLastLinkId()));
            QUrl url(QString("http://%1?%2").arg(settings.linkCheckerUrl()).arg(uQuery.toString()));
            QNetworkReply * reply = nManager.get(QNetworkRequest(url));
            QSharedPointer<QNetworkReply> pointer(reply);
            QEventLoop loop(this);
            connect(&nManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
            loop.exec();
            if (reply->error() != QNetworkReply::NoError)
            {
                errorMessages[i] = tr("Ошибка проверка ссылки: %1").arg(reply->errorString());
                break;
            }
            QRegExp regExp("^OK\\:\\s(\\d+)$");
            QByteArray rData = reply->readAll();
            int pos = regExp.indexIn(rData);
            if (pos != -1)
            {
                //По ссылке переходили
                if (regExp.cap(1) == '1')
                {
                    qDebug() << "По ссылке переходили";

                    GraphicsBlock *nextBlock = treeEdit.nextPositiveBlock(block);
                    if (nextBlock == nullptr)
                    {
                        errorMessages[i] = treeEdit.getErrorString();
                        break;
                    }
                    qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
                    _clientCards[i].setBlockId(nextIdx);
                    _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
                }
                //По ссылке не переходили
                else
                {
                    qDebug() << "По ссылке не переходили";

                    GraphicsBlock *nextBlock = treeEdit.nextNegativeBlock(block);
                    qDebug() << nextBlock->content();
                    if (nextBlock == nullptr)
                    {
                        errorMessages[i] = treeEdit.getErrorString();
                        break;
                    }
                    qint32 nextIdx = treeEdit.getCurrentBlockId(nextBlock);
                    _clientCards[i].setBlockId(nextIdx);
                    _clientCards[i].setBlockExecDateTime(QDateTime::currentDateTime());
                }
                _addLog("green", tr("Проверка ссылки успешна"));
            }
            else
            {
                errorMessages[i] = rData;
                break;
            }
            break;
        }
        default:
            break;
        }
    }
    //error loggin
    bool isNeedMessage(false);
    for(int j = 0 ; j < errorMessages.size(); ++j)
        if (!errorMessages[j].isEmpty())
        {
            isNeedMessage = true;
            break;
        }
    qint32 errorCount(0);
    if (isNeedMessage)
    {
        _addLog("black", tr("Во время выполнения были обнаружены некоторые ошибки"));
        QStringList lst;
        for(int j = 0 ; j < errorMessages.size(); ++j)
            if (!errorMessages[j].isEmpty())
            {
                lst << tr("!ERROR - Client: %1 (%2). %3").arg(j).arg(_clientCards[j].getContactFace()).arg(errorMessages[j]);
                ++errorCount;
            }
        for(int j = 0 ; j < lst.size(); ++j)
        {
            _addLog("red", lst[j]);
        }
    }
    _addLog("blue", tr("Окончили; Всего: %1; Успешно: %2; Ошибок: %3").arg(_clientCards.size()).arg(_clientCards.size()-errorCount).arg(errorCount));
    ui->status_lbl->setText(tr("Окончили"));
}


void MainWindow::on_pushButton_2_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton->setEnabled(false);
        _intervalTimerAmount = 60;
        _intervalTimer->start(1000);
    }
    else
    {
        ui->pushButton->setEnabled(true);
        _intervalTimer->stop();
    }
}

void MainWindow::on_action_VK_triggered()
{
    VkSettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        _vkProcessor->loadSettings();
}

void MainWindow::on_action_VK_2_triggered()
{
    VkAddUserDialog dialog(this);
    dialog.exec();
}
