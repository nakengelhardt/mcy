/*
 *  mcy-gui -- Mutation Cover with Yosys GUI
 *
 *  Copyright (C) 2019  Miodrag Milanovic <miodrag@symbioticeda.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <QtWidgets>
#include "createwizard.h"
#include "intropage.h"
#include "selectdir.h"
#include "designsetup.h"
#include "testsetup.h"

CreateWizard::CreateWizard(QWidget *parent)
    : QWizard(parent)
{
    setPage(Page_Intro, new IntroPage);
    setPage(Page_SelectDirectory, new SelectDirectoryPage);
    setPage(Page_DesignSetup, new DesignSetupPage);
    setPage(Page_TestSetup, new TestSetupPage);

    setStartId(Page_Intro);
#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif
    setOption(HaveHelpButton, true);
    setPixmap(QWizard::LogoPixmap, QPixmap(":/icons/resources/symbiotic.png"));

    connect(this, &QWizard::helpRequested, this, &CreateWizard::showHelp);
    setWindowTitle(tr("Create Wizard"));
}

void CreateWizard::showHelp()
{
    QString message;

    switch (currentId()) {
    case Page_Intro:
        message = tr("TODO: Intro page help message.");
        break;
    case Page_DesignSetup:
        message = tr("TODO: Select files help message.");
        break;
    default:
        message = tr("This help is likely not to be of any help.");
    }

    QMessageBox::information(this, tr("Create Wizard Help"), message);
}

void CreateWizard::accept()
{
    QByteArray content;
    content += "[options]"; content += "\n";
    content += QString("size ") + field("mutations_size").toString(); content += "\n";
   
    content += "\n";
    
    content += "[script]"; content += "\n";
    content += field("script").toString();
    content += "\n";
    content += "\n";

    content += "[files]"; content += "\n";
    QStringList fileList = field("theFileList").toStringList();
    for (auto item : fileList) {
        content += item;
        content += "\n";
    }
    content += "\n";

    content += "[logic]"; content += "\n";
    content += "tag(\"NOC\")"; content += "\n";
    content += "\n";

    QFile headerFile(QDir::cleanPath(field("directory").toString() + QDir::separator() + "config.mcy"));
    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Create Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }
    headerFile.write(content);

    QDialog::accept();
}