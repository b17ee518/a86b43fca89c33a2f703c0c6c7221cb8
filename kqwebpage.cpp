#include "kQWebPage.h"

KQWebPage::KQWebPage(QWidget *parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	: QWebEnginePage(parent)
#else
	: QWebPage(parent)
#endif
{
}
