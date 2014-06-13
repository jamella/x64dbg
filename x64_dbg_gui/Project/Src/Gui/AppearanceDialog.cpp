#include "AppearanceDialog.h"
#include "ui_AppearanceDialog.h"
#include "Bridge.h"

AppearanceDialog::AppearanceDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AppearanceDialog)
{
    ui->setupUi(this);
    //set window flags
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(this->size()); //fixed size
    //Colors
    colorMap=&Configuration::instance()->Colors;
    colorBackupMap=*colorMap;
    ui->groupColor->setEnabled(false);
    ui->groupBackgroundColor->setEnabled(false);
    colorInfoListInit();
}

AppearanceDialog::~AppearanceDialog()
{
    delete ui;
}

//Colors
void AppearanceDialog::on_button000000_clicked()
{
    ui->editColor->setText("#000000");
}

void AppearanceDialog::on_button000080_clicked()
{
    ui->editColor->setText("#000080");
}

void AppearanceDialog::on_button008000_clicked()
{
    ui->editColor->setText("#008000");
}

void AppearanceDialog::on_button008080_clicked()
{
    ui->editColor->setText("#008080");
}

void AppearanceDialog::on_button800000_clicked()
{
    ui->editColor->setText("#800000");
}

void AppearanceDialog::on_button800080_clicked()
{
    ui->editColor->setText("#800080");
}

void AppearanceDialog::on_button808000_clicked()
{
    ui->editColor->setText("#808000");
}

void AppearanceDialog::on_buttonC0C0C0_clicked()
{
    ui->editColor->setText("#C0C0C0");
}

void AppearanceDialog::on_button808080_clicked()
{
    ui->editColor->setText("#808080");
}

void AppearanceDialog::on_button0000FF_clicked()
{
    ui->editColor->setText("#0000FF");
}

void AppearanceDialog::on_button00FF00_clicked()
{
    ui->editColor->setText("#00FF00");
}

void AppearanceDialog::on_button00FFFF_clicked()
{
    ui->editColor->setText("#00FFFF");
}

void AppearanceDialog::on_buttonFF0000_clicked()
{
    ui->editColor->setText("#FF0000");
}

void AppearanceDialog::on_buttonFF00FF_clicked()
{
    ui->editColor->setText("#FF00FF");
}

void AppearanceDialog::on_buttonFFFF00_clicked()
{
    ui->editColor->setText("#FFFF00");
}

void AppearanceDialog::on_buttonFFFFFF_clicked()
{
    ui->editColor->setText("#FFFBF0");
}

void AppearanceDialog::on_buttonBackground000000_clicked()
{
    ui->editBackgroundColor->setText("#000000");
}

void AppearanceDialog::on_buttonBackgroundC0C0C0_clicked()
{
    ui->editBackgroundColor->setText("#C0C0C0");
}

void AppearanceDialog::on_buttonBackgroundFFFFFF_clicked()
{
    ui->editBackgroundColor->setText("#FFFBF0");
}

void AppearanceDialog::on_buttonBackground00FFFF_clicked()
{
    ui->editBackgroundColor->setText("#00FFFF");
}

void AppearanceDialog::on_buttonBackground00FF00_clicked()
{
    ui->editBackgroundColor->setText("#00FF00");
}

void AppearanceDialog::on_buttonBackgroundFF0000_clicked()
{
    ui->editBackgroundColor->setText("#FF0000");
}

void AppearanceDialog::on_buttonBackgroundFFFF00_clicked()
{
    ui->editBackgroundColor->setText("#FFFF00");
}

void AppearanceDialog::on_buttonBackgroundNone_clicked()
{
    ui->editBackgroundColor->setText("#XXXXXX");
}

void AppearanceDialog::on_editBackgroundColor_textChanged(const QString &arg1)
{
    QString text=arg1;
    if(!arg1.length())
    {
        ui->editBackgroundColor->setText("#");
        text=ui->editBackgroundColor->text();
        return;
    }
    if(arg1.at(0)!='#')
    {
        ui->editBackgroundColor->setText("#"+arg1);
        text=ui->editBackgroundColor->text();
    }
    QString styleSheet;
    QString id=colorInfoList.at(colorInfoIndex).backgroundColorName;
    if(text=="#XXXXXX")
    {
        styleSheet = "border: 2px solid black; background-color: #C0C0C0";
        ui->buttonBackgroundColor->setText("X");
        if(colorMap->contains(id))
        {
            (*colorMap)[id]=Qt::transparent;
            ui->buttonSave->setEnabled(true);
            Configuration::instance()->writeColors();
            GuiUpdateAllViews();
        }
    }
    else
    {
        ui->buttonBackgroundColor->setText("");
        if(QColor(text).isValid())
        {
            styleSheet = "border: 2px solid black; background-color: " + text;
            if(colorMap->contains(id))
            {
                (*colorMap)[id]=QColor(text);
                ui->buttonSave->setEnabled(true);
                Configuration::instance()->writeColors();
                GuiUpdateAllViews();
            }
        }
        else
        {
            styleSheet = "border: 2px solid red; background-color: #FFFFFF";
            if(colorMap->contains(id))
                ui->buttonSave->setEnabled(false); //we cannot save with an invalid color
        }
    }
    ui->buttonBackgroundColor->setStyleSheet(styleSheet);
}

void AppearanceDialog::on_editColor_textChanged(const QString &arg1)
{
    QString text=arg1;
    if(!arg1.length())
    {
        ui->editColor->setText("#");
        text=ui->editColor->text();
        return;
    }
    if(arg1.at(0)!='#')
    {
        ui->editColor->setText("#"+arg1);
        text=ui->editColor->text();
    }
    QString id=colorInfoList.at(colorInfoIndex).colorName;
    QString styleSheet;
    if(QColor(text).isValid())
    {
        styleSheet = "border: 2px solid black; background-color: " + text;
        if(colorMap->contains(id))
        {
            (*colorMap)[id]=QColor(text);
            ui->buttonSave->setEnabled(true);
            Configuration::instance()->writeColors();
            GuiUpdateAllViews();
        }
    }
    else
    {
        styleSheet = "border: 2px solid red; background-color: #FFFFFF";
        if(colorMap->contains(id))
            ui->buttonSave->setEnabled(false); //we cannot save with an invalid color
    }
    ui->buttonColor->setStyleSheet(styleSheet);
}

void AppearanceDialog::on_buttonColor_clicked()
{
    QColorDialog colorDialog(QColor(ui->editColor->text()), this);
    if(colorDialog.exec()==QDialog::Accepted)
        ui->editColor->setText(colorDialog.selectedColor().name().toUpper());
}

void AppearanceDialog::on_buttonBackgroundColor_clicked()
{
    QColor initialColor;
    if(ui->editBackgroundColor->text().toUpper()=="#XXXXXX")
        initialColor=Qt::black; //transparent will set the alpha channel, which users will forget
    else
        initialColor=QColor(ui->editBackgroundColor->text());
    QColor selectedColor=QColorDialog::getColor(initialColor, this, "Select Color", QColorDialog::ShowAlphaChannel);
    if(selectedColor.isValid())
    {
        if(!selectedColor.alpha())
            ui->editBackgroundColor->setText("#XXXXXX");
        else
            ui->editBackgroundColor->setText(selectedColor.name().toUpper());
    }
}

void AppearanceDialog::on_listColorNames_itemSelectionChanged()
{
    colorInfoIndex=ui->listColorNames->row(ui->listColorNames->selectedItems().at(0));
    ColorInfo info=colorInfoList.at(colorInfoIndex);
    defaultValueAction->setEnabled(false);
    currentSettingAction->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    ui->groupColor->setEnabled(false);
    ui->groupBackgroundColor->setEnabled(false);

    if(info.colorName.length())
    {
        QString id=info.colorName;
        if(colorMap->contains(id))
        {
            ui->groupColor->setEnabled(true);
            ui->buttonSave->setEnabled(true);
            defaultValueAction->setEnabled(true);
            currentSettingAction->setEnabled(true);

            QColor color=(*colorMap)[id];
            QString colorText=color.name().toUpper();
            if(!color.alpha())
                colorText="#XXXXXX";
            ui->editColor->setText(colorText);
        }
        else
            ui->editColor->setText("#FFFFFF");
    }
    else
        ui->editColor->setText("#FFFFFF");

    if(info.backgroundColorName.length())
    {
        QString id=info.backgroundColorName;
        if(colorMap->contains(id))
        {
            ui->groupBackgroundColor->setEnabled(true);
            ui->buttonSave->setEnabled(true);
            defaultValueAction->setEnabled(true);
            currentSettingAction->setEnabled(true);

            QColor color=(*colorMap)[id];
            QString colorText=color.name().toUpper();
            if(!color.alpha())
                colorText="#XXXXXX";
            ui->editBackgroundColor->setText(colorText);
        }
        else
            ui->editBackgroundColor->setText("#FFFFFF");
    }
    else
        ui->editBackgroundColor->setText("#FFFFFF");
}

void AppearanceDialog::on_buttonSave_clicked()
{
    Configuration::instance()->writeColors();
    QMessageBox msg(QMessageBox::Information, "Information", "Settings saved!");
    msg.setWindowIcon(QIcon(":/icons/images/information.png"));
    msg.setParent(this, Qt::Dialog);
    msg.setWindowFlags(msg.windowFlags()&(~Qt::WindowContextHelpButtonHint));
    msg.exec();
    GuiUpdateAllViews();
}

void AppearanceDialog::on_buttonCancel_clicked()
{
    Configuration::instance()->Colors=colorBackupMap;
    Configuration::instance()->writeColors();
    GuiUpdateAllViews();
}

void AppearanceDialog::defaultValueSlot()
{
    ColorInfo info=colorInfoList.at(colorInfoIndex);
    if(info.colorName.length())
    {
        QString id=info.colorName;
        if(Configuration::instance()->defaultColors.contains(id))
        {
            QColor color=Configuration::instance()->defaultColors[id];
            QString colorText=color.name().toUpper();
            if(!color.alpha())
                colorText="#XXXXXX";
            ui->editColor->setText(colorText);
        }
    }
    if(info.backgroundColorName.length())
    {
        QString id=info.backgroundColorName;
        if(Configuration::instance()->defaultColors.contains(id))
        {
            QColor color=Configuration::instance()->defaultColors[id];
            QString colorText=color.name().toUpper();
            if(!color.alpha())
                colorText="#XXXXXX";
            ui->editBackgroundColor->setText(colorText);
        }
    }
}

void AppearanceDialog::currentSettingSlot()
{
    ColorInfo info=colorInfoList.at(colorInfoIndex);
    if(info.colorName.length())
    {
        QString id=info.colorName;
        if(colorBackupMap.contains(id))
        {
            QColor color=colorBackupMap[id];
            QString colorText=color.name().toUpper();
            if(!color.alpha())
                colorText="#XXXXXX";
            ui->editColor->setText(colorText);
        }
    }
    if(info.backgroundColorName.length())
    {
        QString id=info.backgroundColorName;
        if(colorBackupMap.contains(id))
        {
            QColor color=colorBackupMap[id];
            QString colorText=color.name().toUpper();
            if(!color.alpha())
                colorText="#XXXXXX";
            ui->editBackgroundColor->setText(colorText);
        }
    }
}

void AppearanceDialog::colorInfoListAppend(QString propertyName, QString colorName, QString backgroundColorName)
{
    ColorInfo info;
    if(colorName.length() || backgroundColorName.length())
        propertyName="     "+propertyName;
    else
        propertyName=QString(QChar(0x2022))+" "+propertyName; //bullet + space
    info.propertyName=propertyName;
    info.colorName=colorName;
    info.backgroundColorName=backgroundColorName;
    colorInfoList.append(info);
    ui->listColorNames->addItem(colorInfoList.last().propertyName);
}

void AppearanceDialog::colorInfoListInit()
{
    //clear list
    colorInfoIndex=0;
    colorInfoList.clear();
    //list entries
    colorInfoListAppend("AbstractTableView:", "", "");
    colorInfoListAppend("Text", "AbstractTableViewTextColor", "");
    colorInfoListAppend("Header Text", "AbstractTableViewHeaderTextColor", "");
    colorInfoListAppend("Background", "AbstractTableViewBackgroundColor", "");
    colorInfoListAppend("Selection", "AbstractTableViewSelectionColor", "");
    colorInfoListAppend("Separators", "AbstractTableViewSeparatorColor", "");

    colorInfoListAppend("Disassembly:", "", "");
    colorInfoListAppend("Background", "DisassemblyBackgroundColor", "");
    colorInfoListAppend("Selection", "DisassemblySelectionColor", "");
    colorInfoListAppend("Bytes", "DisassemblyBytesColor", "");
#ifdef _WIN64
    colorInfoListAppend("RIP", "DisassemblyCipColor", "DisassemblyCipBackgroundColor");
#else //x86
    colorInfoListAppend("EIP", "DisassemblyCipColor", "DisassemblyCipBackgroundColor");
#endif //_WIN64
    colorInfoListAppend("Breakpoints", "DisassemblyBreakpointColor", "DisassemblyBreakpointBackgroundColor");
    colorInfoListAppend("Hardware Breakpoints", "DisassemblyHardwareBreakpointColor", "DisassemblyHardwareBreakpointBackgroundColor");
    colorInfoListAppend("Bookmarks", "DisassemblyBookmarkColor", "DisassemblyBookmarkBackgroundColor");
    colorInfoListAppend("Comments", "DisassemblyCommentColor", "DisassemblyCommentBackgroundColor");
    colorInfoListAppend("Labels", "DisassemblyLabelColor", "DisassemblyLabelBackgroundColor");
    colorInfoListAppend("Addresses", "DisassemblyAddressColor", "DisassemblyAddressBackgroundColor");
    colorInfoListAppend("Selected Addresses", "DisassemblySelectedAddressColor", "DisassemblySelectedAddressBackgroundColor");
    colorInfoListAppend("Conditional Jump Lines (jump)", "DisassemblyConditionalJumpLineTrueColor", "");
    colorInfoListAppend("Conditional Jump Lines (no jump)", "DisassemblyConditionalJumpLineFalseColor", "");
    colorInfoListAppend("Unconditional Jump Lines", "DisassemblyUnconditionalJumpLineColor", "");

    colorInfoListAppend("SideBar:", "", "");
#ifdef _WIN64
    colorInfoListAppend("RIP Label", "SideBarCipLabelColor", "SideBarCipLabelBackgroundColor");
#else //x86
    colorInfoListAppend("EIP Label", "SideBarCipLabelColor", "SideBarCipLabelBackgroundColor");
#endif //_WIN64
    colorInfoListAppend("Bullets", "SideBarBulletColor", "");
    colorInfoListAppend("Breakpoints", "SideBarBulletBreakpointColor", "");
    colorInfoListAppend("Disabled Breakpoints", "SideBarBulletDisabledBreakpointColor", "");
    colorInfoListAppend("Bookmarks", "SideBarBulletBookmarkColor", "");
    colorInfoListAppend("Conditional Jump Lines (jump)", "SideBarConditionalJumpLineTrueColor", "");
    colorInfoListAppend("Conditional Jump Lines (no jump)", "SideBarConditionalJumpLineFalseColor", "");
    colorInfoListAppend("Unconditional Jump Lines (jump)", "SideBarUnconditionalJumpLineTrueColor", "");
    colorInfoListAppend("Unconditional Jump Lines (no jump)", "SideBarUnconditionalJumpLineFalseColor", "");
    colorInfoListAppend("Jump Lines (executing)", "SideBarJumpLineExecuteColor", "");
    colorInfoListAppend("Background", "SideBarBackgroundColor", "");

    colorInfoListAppend("Registers:", "", "");
    colorInfoListAppend("Text", "RegistersColor", "");
    colorInfoListAppend("Background", "RegistersBackgroundColor", "");
    colorInfoListAppend("Selection", "RegistersSelectionColor", "");
    colorInfoListAppend("Modified Registers", "RegistersModifiedColor", "");
    colorInfoListAppend("Name of Labels", "RegistersLabelColor", "");
    colorInfoListAppend("Extra Information", "RegistersExtraInfoColor", "");

    colorInfoListAppend("Instructions:", "", "");
    colorInfoListAppend("Text", "InstructionUncategorizedColor", "InstructionUncategorizedBackgroundColor");
    colorInfoListAppend("Commas", "InstructionCommaColor", "InstructionCommaBackgroundColor");
    colorInfoListAppend("Prefixes", "InstructionPrefixColor", "InstructionPrefixBackgroundColor");
    colorInfoListAppend("Addresses", "InstructionAddressColor", "InstructionAddressBackgroundColor");
    colorInfoListAppend("Values", "InstructionValueColor", "InstructionValueBackgroundColor");
    colorInfoListAppend("Mnemonics", "InstructionMnemonicColor", "InstructionMnemonicBackgroundColor");
    colorInfoListAppend("Push/Pops", "InstructionPushPopColor", "InstructionPushPopBackgroundColor");
    colorInfoListAppend("Calls", "InstructionCallColor", "InstructionCallBackgroundColor");
    colorInfoListAppend("Returns", "InstructionRetColor", "InstructionRetBackgroundColor");
    colorInfoListAppend("Conditional Jumps", "InstructionConditionalJumpColor", "InstructionConditionalJumpBackgroundColor");
    colorInfoListAppend("Unconditional Jumps", "InstructionUnconditionalJumpColor", "InstructionUnconditionalJumpBackgroundColor");
    colorInfoListAppend("NOPs", "InstructionNopColor", "InstructionNopBackgroundColor");
    colorInfoListAppend("General Registers", "InstructionGeneralRegisterColor", "InstructionGeneralRegisterBackgroundColor");
    colorInfoListAppend("FPU Registers", "InstructionFpuRegisterColor", "InstructionFpuRegisterBackgroundColor");
    colorInfoListAppend("SSE Registers", "InstructionSseRegisterColor", "InstructionSseRegisterBackgroundColor");
    colorInfoListAppend("MMX Registers", "InstructionMmxRegisterColor", "InstructionMmxRegisterBackgroundColor");
    colorInfoListAppend("Memory Sizes", "InstructionMemorySizeColor", "InstructionMemorySizeBackgroundColor");
    colorInfoListAppend("Memory Segments", "InstructionMemorySegmentColor", "InstructionMemorySegmentBackgroundColor");
    colorInfoListAppend("Memory Brackets", "InstructionMemoryBracketsColor", "InstructionMemoryBracketsBackgroundColor");
    colorInfoListAppend("Memory Stack Brackets", "InstructionMemoryStackBracketsColor", "InstructionMemoryStackBracketsBackgroundColor");
    colorInfoListAppend("Memory Base Registers", "InstructionMemoryBaseRegisterColor", "InstructionMemoryBaseRegisterBackgroundColor");
    colorInfoListAppend("Memory Index Registers", "InstructionMemoryIndexRegisterColor", "InstructionMemoryIndexRegisterBackgroundColor");
    colorInfoListAppend("Memory Scales", "InstructionMemoryScaleColor", "InstructionMemoryScaleBackgroundColor");
    colorInfoListAppend("Memory Operators (+/-/*)", "InstructionMemoryOperatorColor", "InstructionMemoryOperatorBackgroundColor");

    colorInfoListAppend("HexDump:", "", "");
    colorInfoListAppend("Text", "HexDumpTextColor", "");
    colorInfoListAppend("Background", "HexDumpBackgroundColor", "");
    colorInfoListAppend("Selection", "HexDumpSelectionColor", "");
    colorInfoListAppend("Addresses", "HexDumpAddressColor", "HexDumpAddressBackgroundColor");
    colorInfoListAppend("Labels", "HexDumpLabelColor", "HexDumpLabelBackgroundColor");

    colorInfoListAppend("Stack:", "", "");
    colorInfoListAppend("Text", "StackTextColor", "");
    colorInfoListAppend("Inactive Text", "StackInactiveTextColor", "");
    colorInfoListAppend("Background", "StackBackgroundColor", "");
    colorInfoListAppend("Selection", "StackSelectionColor", "");
#ifdef _WIN64
    colorInfoListAppend("RSP", "StackCspColor", "StackCspBackgroundColor");
#else //x86
    colorInfoListAppend("CSP", "StackCspColor", "StackCspBackgroundColor");
#endif //_WIN64
    colorInfoListAppend("Addresses", "StackAddressColor", "StackAddressBackgroundColor");
    colorInfoListAppend("Selected Addresses", "StackSelectedAddressColor", "StackSelectedAddressBackgroundColor");
    colorInfoListAppend("Labels", "StackLabelColor", "StackLabelBackgroundColor");

    colorInfoListAppend("Other:", "", "");
    colorInfoListAppend("Current Thread", "ThreadCurrentColor", "ThreadCurrentBackgroundColor");

    //dev helper
    const QMap<QString, QColor>* Colors=&Configuration::instance()->defaultColors;
    QString notFound;
    for(int i=0; i<Colors->size(); i++)
    {
        QString id=Colors->keys().at(i);
        bool bFound=false;
        for(int j=0; j<colorInfoList.size(); j++)
        {
            if(colorInfoList.at(j).colorName==id || colorInfoList.at(j).backgroundColorName==id)
            {
                bFound=true;
                break;
            }
        }
        if(!bFound) //color not found in info list
            notFound+=id+"\n";
    }
    if(notFound.length())
    {
        QMessageBox msg(QMessageBox::Warning, "NOT FOUND IN CONFIG!", notFound);
        msg.setWindowIcon(QIcon(":/icons/images/compile-warning.png"));
        msg.setParent(this, Qt::Dialog);
        msg.setWindowFlags(msg.windowFlags()&(~Qt::WindowContextHelpButtonHint));
        msg.exec();
    }

    //setup context menu
    ui->listColorNames->setContextMenuPolicy(Qt::ActionsContextMenu);
    defaultValueAction = new QAction("&Default Value", this);
    defaultValueAction->setEnabled(false);
    connect(defaultValueAction, SIGNAL(triggered()), this, SLOT(defaultValueSlot()));
    currentSettingAction = new QAction("&Current Setting", this);
    currentSettingAction->setEnabled(false);
    connect(currentSettingAction, SIGNAL(triggered()), this, SLOT(currentSettingSlot()));
    ui->listColorNames->addAction(defaultValueAction);
    ui->listColorNames->addAction(currentSettingAction);
}