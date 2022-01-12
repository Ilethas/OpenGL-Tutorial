#include "MovementSanitizer.h"


MovementSanitizer::MovementSanitizer()
{

}


QVector3D MovementSanitizer::movementVector() const
{
    QVector3D movement;
    movement.setX((movingForward? 1 : 0) + (movingBackward? -1 : 0));
    movement.setY((movingRight? 1 : 0) + (movingLeft? -1 : 0));
    movement.setZ((movingUp? 1 : 0) + (movingDown? -1 : 0));
    return movement.normalized();
}
