/************************************************************************
**
** Authors:   Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve), 
**            Marek Krejza <krejza@gmail.com> (Caligor)
**
** This file is part of the MMapper2 project. 
** Maintained by Marek Krejza <krejza@gmail.com>
**
** Copyright: See COPYING file that comes with this distribution
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file COPYING included in the packaging of
** this file.  
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
*************************************************************************/

#include <QtGui>

#include "configdialog.h"
#include "generalpage.h"
#include "colorfontpage.h"

ConfigDialog::ConfigDialog()
{
  contentsWidget = new QListWidget;
  contentsWidget->setViewMode(QListView::IconMode);
  contentsWidget->setIconSize(QSize(70, 70));
  contentsWidget->setMovement(QListView::Static);
  contentsWidget->setMaximumWidth(90);
  contentsWidget->setMinimumWidth(90);
  contentsWidget->setSpacing(9);

  pagesWidget = new QStackedWidget;
  pagesWidget->setMinimumWidth(400);
  pagesWidget->setMinimumHeight(480);

  QPushButton *closeButton = new QPushButton(tr("Close"));

  createIcons();

  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(contentsWidget);
  horizontalLayout->addWidget(pagesWidget, 1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(closeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addStretch(1);
  mainLayout->addSpacing(12);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);

  setWindowTitle(tr("Config Dialog"));

  pagesWidget->addWidget(new GeneralPage);
  pagesWidget->setCurrentIndex(0);
}

void ConfigDialog::createIcons()
{
  QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
  configButton->setIcon(QIcon(":/icons/page.png"));
  configButton->setText(tr("General"));
  configButton->setTextAlignment(Qt::AlignHCenter);
  configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *colorFontButton = new QListWidgetItem(contentsWidget);
  colorFontButton->setIcon(QIcon(":/icons/page.png"));
  colorFontButton->setText(tr("Color and\nFont"));
  colorFontButton->setTextAlignment(Qt::AlignHCenter);
  colorFontButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
          this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)) );
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!current)
    current = previous;

  if (pagesWidget->count()<=1)
  {
    pagesWidget->addWidget(new ColorFontPage);
    //pagesWidget->addWidget(new GroupManagerPage(m_groupManager));
  }

  pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

