#ifndef MOVEMENTSANITIZER_H
#define MOVEMENTSANITIZER_H
#include <QVector3D>


class MovementSanitizer
{
    public:
        bool movingForward = false;
        bool movingBackward = false;
        bool movingRight = false;
        bool movingLeft = false;
        bool movingUp = false;
        bool movingDown = false;

        MovementSanitizer();
        QVector3D movementVector() const;
};


#endif // MOVEMENTSANITIZER_H
