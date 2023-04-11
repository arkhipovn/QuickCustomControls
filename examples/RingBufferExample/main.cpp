#include <QCoreApplication>
#include <CRingBuffer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CRingBuffer <int> buffer(3);
    buffer.append(1);
    buffer.append(2);
    buffer.append(3);
    buffer.append(4);
    buffer.append(5);
    buffer.append(6);
    buffer.pop();
    buffer.pop();
    buffer.pop();
    buffer.append(7);
    buffer.append(8);
    buffer.append(9);
    buffer.append(10);
    //    buffer.pop();
    //    qDebug() << "pop" << buffer.pop() << buffer.pop() << buffer.pop()<< buffer.pop()<< buffer.pop();

    qDebug() << "vector" << buffer.toVector();
    qDebug() << buffer.size() << buffer.bufferSize();
    qDebug() << buffer[0] << buffer[1] << buffer[2];
    return a.exec();
}
//QVector
