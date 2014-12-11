TEMPLATE = subdirs
SUBDIRS = components

symbian3 {
    SUBDIRS += symbian
    extras:SUBDIRS += symbian/extras
}
