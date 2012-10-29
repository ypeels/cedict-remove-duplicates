#include <QDebug>
#include <QFile> // unicode!!!
#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>

static QString const INPUT_FILE = "trad.tab";
static QString const OUTPUT_FILE = "trad.weeded.tab";

int main()
{
    qDebug() << "hello world from qdebug";
    
// Input step 1: initialize files and streams
    // http://qt-project.org/doc/qt-4.8/qfile.html
    QFile inFile(INPUT_FILE);
    if ( !inFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        qWarning("could not open input file");
        return 1;
    }
    QTextStream inStream(&inFile);
    inStream.setCodec("UTF-8");
    
    
// Input Step 2. populate hash, merging any duplicate entries   
    QString inLine;
    QHash<QString, QString> dictionary;
    while (!inFile.atEnd())
    {
        inLine = inStream.readLine();
        QStringList inList = inLine.split("\t");
        Q_ASSERT(inList.size() == 2);
        
        QString word = inList[0];
        QString definition = inList[1];
        
        // merge duplicate entry's definition
        if (dictionary.contains(word))
            definition = dictionary.value(word) + definition;

        // check (word, def) pair into dictionary
        dictionary.insert(word, definition);        
    }
 

// Output step 1: initialize output stream 
    QFile outFile(OUTPUT_FILE);
    if ( !outFile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        qWarning("could not open output file");
        return 1;
    }
    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8"); // http://www.qtforum.org/article/26934/reading-from-utf-8-file.html?s=820722474344b610ba266cc0948d747b8f2e3eda#post93509
 
// Output step 2: output weeded dictionary to file
    QList<QString> wordList = dictionary.keys();
    for(int i = 0; i < wordList.length(); i++)
    {
        QString word = wordList.at(i);
        outStream << word << '\t' << dictionary[word] << '\r';
    }
    
    
    
    qDebug() << "goodbye world";
    return 0;
}
