//
// Created by Luke Martin on 2023-04-06.
//

#include "InputTable.h"


// ----- Ctor, Dtor, and Other Important Functions ----- //
InputTable::InputTable() {
    this->tableLayout = new QGridLayout;
    for (int i = 0; i < 81; i++) {
        auto* newBox = new QLineEdit;
        newBox->setStyleSheet(this->settingSS); // This is the default stylesheet
        newBox->setValidator(this->forceTheseValuesWhenSetting); // This is the default validator.
        connect(newBox, &QLineEdit::textChanged, this, &InputTable::returnUpdatedTileValue);
        this->tileInputBoxes[i] = newBox;
        this->tableLayout->addWidget(newBox, i - (i % 9), i % 9, 1, 1);
    }
    this->tableLayout->setSpacing(0);
    this->tableLayout->setContentsMargins(0,0,0,0);
}

// ----- Getters ----- //
std::map<int, int> InputTable::getInputTableValues() const {
    std::map<int, int> tableValues;
    for (auto & tiles : this->tileInputBoxes) {
        tiles.second->text().isEmpty() ? tableValues[tiles.first] = -1 : tableValues[tiles.first] = tiles.second->text().toInt(); // NOTE: this is guaranteed to be a safe type change because the regexes never allow non-numeric characters.
    }
    return tableValues;
}

bool InputTable::getSettingMode() const {
    return this->settingMode;
}

// ----- Setters ----- //
void InputTable::setCurrentTable(SudokuTable* newTable) { // TODO: add possibilities
    this->currentTable = newTable;
    for (auto &it : this->currentTable->getTileValues()) {
        if (it.second != -1) {
            this->tileInputBoxes[it.first]->setText(QString::number(it.second));
        } else {
            this->tileInputBoxes[it.first]->setText(QString (""));
        }
    }
}

// ----- Pass-off from InputWindow's Slots ----- //
void InputTable::saveTable(std::ofstream &file){ // TODO: add possibilities
    if (this->currentTable) {
        this->currentTable->updateTable(this->getInputTableValues());
    } else {
        this->currentTable = new SudokuTable();
    }
    for (auto & tiles : this->currentTable->getTileValues()) {
        file << std::to_string(tiles.first) << " " << std::to_string(tiles.second) << std::endl;
    }
}

// ----- Validation Functions ----- //
bool InputTable::validateLength(const QString& values) const {
    return (this->settingMode ? values.length() <= 1 : values.length() <= 9);
}

int InputTable::validateNoDuplicates(const std::string &values) {
    for (int i = 0; i < values.length(); i++) {
        if (i != values.length() - 1 && values.at(i) == values.at(i+1)) {
            return i;
        }
    }
    return -1;
}

// ----- Slot Definitions ----- //
int InputTable::returnUpdatedTileValue(const QString& newVal) {
    // TODO: Allow for formatting based on input mode for passing to ST.
    auto* box = qobject_cast<QLineEdit*>(sender());
    if (this->settingMode && !this->validateLength(newVal)) {
        QString newSetValue = newVal.at(newVal.length()-1);
        box->setValidator(this->forceTheseValuesWhenSetting);
        box->setStyleSheet(this->settingSS);
        box->setText(newSetValue);
    } else if (this->settingMode) { // NOTE: this should run if this->validateLength returns true. It was not included for brevity.
        box->setStyleSheet(this->settingSS);
    } else {
        box->setStyleSheet(this->possibilitiesSS);
        std::string sortableNewVal = newVal.toStdString(); // Had bad access issues, just converted to string instead.
        std::cout << sortableNewVal << std::endl;
        sort(sortableNewVal.begin(), sortableNewVal.end()); // Sorts the string to make duplicate checking possible and cleans the UI.
        std::cout << sortableNewVal << std::endl;
        int duplicateIndex = this->validateNoDuplicates(sortableNewVal);
        if (duplicateIndex != -1) {
            box->setText(QString::fromStdString(sortableNewVal.substr(0, duplicateIndex) + sortableNewVal.substr(duplicateIndex+1, sortableNewVal.length()-1)));
        } else {
            box->setText(QString::fromStdString(sortableNewVal));
        }
    }
    return newVal.toInt();
}

void InputTable::switchMode() {
    this->settingMode = !this->settingMode;
    for (auto & boxes : this->tileInputBoxes) {
        if (this->settingMode && this->validateLength(boxes.second->text())) {
            boxes.second->setValidator(this->forceTheseValuesWhenSetting);
        } else {
            boxes.second->setValidator(this->forceTheseValuesWhenPossibilitying);
        }
    }
}