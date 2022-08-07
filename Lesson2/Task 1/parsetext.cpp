#include "parsetext.h"

#define MIN(a, b) (a < b) ? a : b


ParseText::ParseText()
{
    text = "";
    preText = "";
    pos = 0;
}

bool ParseText::change(QString istr)
{
    qint32 length = MIN(MIN(text.length(), istr.length()), pos);
    preText = "";
    for (qint32 i = 0; i < length; i++)
    {
        if (text[i] != istr[i])
        {
            pos = i;
            break;
        }
    }
    const QChar *arStr = istr.constData();
    qint32 ppos = 0, lastp = -1;
    bool chg = false;
    qint32 fnd = 0;
    QString symbol, meaning;
    if(istr.indexOf("#RUB@", pos) != -1 || istr.indexOf("#руб@", pos) != -1) {
        meaning = "₽";
        if(istr.indexOf("#RUB@", pos) != -1)symbol = "#RUB@";
        else if(istr.indexOf("#руб@", pos) != -1) symbol = "#руб@";
    }
    else if(istr.indexOf("#C@", pos) != -1 || istr.indexOf("#копи@", pos) != -1){
        meaning = "©";
        if(istr.indexOf("#C@", pos) != -1) symbol = "#C@";
        else if(istr.indexOf("#копи@", pos) != -1) symbol = "#копи@";
    }
    else if(istr.indexOf("#PERM@", pos) != -1 || istr.indexOf("#пром@", pos) != -1){
        meaning = "‰";
        if(istr.indexOf("#PERM@", pos) != -1) symbol = "#PERM@";
        else if(istr.indexOf("#пром@", pos) != -1) symbol = "#пром@";
    }
    else if(istr.indexOf("#PERM@", pos) != -1 || istr.indexOf("#пром@", pos) != -1){
        meaning = "‰";
        if(istr.indexOf("#PERM@", pos) != -1) symbol = "#PERM@";
        else if(istr.indexOf("#пром@", pos) != -1) symbol = "#пром@";
    }
    else if(istr.indexOf("#REG@", pos) != -1 || istr.indexOf("#рег@", pos) != -1){
        meaning = "®";
        if(istr.indexOf("#REG@", pos) != -1) symbol = "#REG@";
        else if(istr.indexOf("#рег@", pos)!= -1) symbol = "#рег@";
    }
    else if(istr.indexOf("#EURO@", pos) != -1 || istr.indexOf("#евро@", pos) != -1){
        meaning = "€";
        if(istr.indexOf("#EURO@", pos) != -1) symbol = "#EURO@";
        else if(istr.indexOf("#евро@", pos) != -1) symbol = "#евро@";
    }


    for (fnd = istr.indexOf(symbol, pos);
        fnd != -1 && lastp != pos;
        fnd = istr.indexOf(symbol, pos)
        )
    {
        preText.insert(preText.length(), &arStr[ppos], fnd);
        lastp = pos;
        pos = fnd;
        int r = istr.indexOf('@', fnd);
        int space = istr.indexOf(' ', fnd);
        if ((r < space || space == -1) && r != -1)
        {
            QString exampl = "";
            exampl.insert(0, &arStr[fnd + 2], r - fnd - 1);
            preText += meaning;
            pos = r + 1;
            chg = true;
        }
        preText.insert(preText.length(), &arStr[pos], istr.length() - pos);
    }
    return chg;
}


QString ParseText::getText()
{

    return preText;
}
