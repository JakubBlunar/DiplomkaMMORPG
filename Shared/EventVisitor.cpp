#include "stdafx.h"
#include "EventVisitor.h"


EventVisitor::EventVisitor() {

}

void EventVisitor::visit(EventMovementChange* e) {
}

void EventVisitor::visit(EventLoginRequest* e) {
}

void EventVisitor::visit(EventLoginResponse* e) {
}

void EventVisitor::visit(EventCharacterChoose* e) {
}

void EventVisitor::visit(EventCharacterChooseResponse* e) {
}

void EventVisitor::visit(EventCharacterMapJoin * e)
{
}

void EventVisitor::visit(EventCharacterMapLeave * e)
{
}

void EventVisitor::visit(EventCharacterLogout * e)
{
}

void EventVisitor::visit(EventNpcMovementChange * e)
{
}

void EventVisitor::visit(EventNpcsMovementChange* e) {}
