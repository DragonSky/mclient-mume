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
#include "generalpage.h"
#include "configuration.h"

GeneralPage::GeneralPage(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  connect( remoteName, SIGNAL( textChanged(const QString&) ), this, SLOT( remoteNameTextChanged(const QString&) )  );
  connect( remotePort, SIGNAL( valueChanged(int) ), this, SLOT( remotePortValueChanged(int) )  );
  connect( localPort, SIGNAL( valueChanged(int) ), this, SLOT( localPortValueChanged(int) )  );

  connect ( acceptBestRelativeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(acceptBestRelativeDoubleSpinBoxValueChanged(double)) );
  connect ( acceptBestAbsoluteDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(acceptBestAbsoluteDoubleSpinBoxValueChanged(double)) );
  connect ( newRoomPenaltyDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(newRoomPenaltyDoubleSpinBoxValueChanged(double)) );
  connect ( correctPositionBonusDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(correctPositionBonusDoubleSpinBoxValueChanged(double)) );
  connect ( multipleConnectionsPenaltyDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(multipleConnectionsPenaltyDoubleSpinBoxValueChanged(double)) );
        
  connect ( maxPaths, SIGNAL(valueChanged(int)), this, SLOT(maxPathsValueChanged(int)) );
  connect ( matchingToleranceSpinBox, SIGNAL(valueChanged(int)), this, SLOT(matchingToleranceSpinBoxValueChanged(int)) );

  connect ( brief, SIGNAL(stateChanged(int)),SLOT(briefStateChanged(int)));
  connect ( emulatedExits, SIGNAL(stateChanged(int)),SLOT(emulatedExitsStateChanged(int)));
  connect ( updated, SIGNAL(stateChanged(int)),SLOT(updatedStateChanged(int)));
  connect ( drawNotMappedExits, SIGNAL(stateChanged(int)),SLOT(drawNotMappedExitsStateChanged(int)));
  connect ( drawUpperLayersTextured, SIGNAL(stateChanged(int)),SLOT(drawUpperLayersTexturedStateChanged(int)));

  connect( autoLoadFileName, SIGNAL( textChanged(const QString&) ), this, SLOT( autoLoadFileNameTextChanged(const QString&) )  );
  connect( autoLoadCheck, SIGNAL(stateChanged(int)),SLOT(autoLoadCheckStateChanged(int)));
  connect( logFileName, SIGNAL( textChanged(const QString&) ), this, SLOT( logFileNameTextChanged(const QString&) )  );
  connect( logCheck, SIGNAL(stateChanged(int)),SLOT(logCheckStateChanged(int)));

  connect( sellectWorldFileButton, SIGNAL(clicked()), this, SLOT(sellectWorldFileButtonClicked()));
  connect( sellectLogFileButton, SIGNAL(clicked()), this, SLOT(sellectLogFileButtonClicked()));

  remoteName->setText( Config().m_remoteServerName );
  remotePort->setValue( Config().m_remotePort );
  localPort->setValue( Config().m_localPort );

  acceptBestRelativeDoubleSpinBox->setValue(Config().m_acceptBestRelative);
  acceptBestAbsoluteDoubleSpinBox->setValue(Config().m_acceptBestAbsolute);
  newRoomPenaltyDoubleSpinBox->setValue(Config().m_newRoomPenalty);
  correctPositionBonusDoubleSpinBox->setValue(Config().m_correctPositionBonus);
  maxPaths->setValue(Config().m_maxPaths);
  matchingToleranceSpinBox->setValue(Config().m_matchingTolerance);
  multipleConnectionsPenaltyDoubleSpinBox->setValue(Config().m_multipleConnectionsPenalty);

  brief->setChecked( Config().m_brief );
  emulatedExits->setChecked( Config().m_emulatedExits );
  updated->setChecked( Config().m_showUpdated );
  drawNotMappedExits->setChecked( Config().m_drawNotMappedExits );
  drawUpperLayersTextured->setChecked( Config().m_drawUpperLayersTextured );

  autoLoadCheck->setChecked( Config().m_autoLoadWorld ); 
  autoLoadFileName->setText( Config().m_autoLoadFileName );
  logCheck->setChecked( Config().m_autoLog ); 
  logFileName->setText( Config().m_logFileName );

}


void GeneralPage::sellectWorldFileButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this,"Choose map file ...","","MMapper2 (*.mm2);;MMapper (*.map)");
  if (!fileName.isEmpty())
  {
    autoLoadFileName->setText( fileName );
    Config().m_logFileName = fileName;   
    Config().m_autoLoadFileName = fileName;  
    autoLoadCheck->setChecked(true);        
    Config().m_autoLoadWorld = true;
  }
}

void GeneralPage::sellectLogFileButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this,"Choose log file ...","","Log (*.log);;All (*.*)");
  if (!fileName.isEmpty())
  {
    logFileName->setText( fileName );
    Config().m_logFileName = fileName;  
    logCheck->setChecked(true);
    Config().m_autoLog = true;      
  }
}

void GeneralPage::remoteNameTextChanged(const QString&)
{
  Config().m_remoteServerName = remoteName->text();       
}

void GeneralPage::remotePortValueChanged(int)
{
  Config().m_remotePort = remotePort->value();
}

void GeneralPage::localPortValueChanged(int)
{
  Config().m_localPort = localPort->value();
}

void GeneralPage::acceptBestRelativeDoubleSpinBoxValueChanged(double val)
{
  Config().m_acceptBestRelative = val;
}

void GeneralPage::acceptBestAbsoluteDoubleSpinBoxValueChanged(double val)
{
  Config().m_acceptBestAbsolute = val;    
}

void GeneralPage::newRoomPenaltyDoubleSpinBoxValueChanged(double val)
{
  Config().m_newRoomPenalty = val;        
}

void GeneralPage::correctPositionBonusDoubleSpinBoxValueChanged(double val)
{
  Config().m_correctPositionBonus = val;  
}

void GeneralPage::multipleConnectionsPenaltyDoubleSpinBoxValueChanged(double val)
{
  Config().m_multipleConnectionsPenalty = val;                    
}

void GeneralPage::maxPathsValueChanged(int val)
{
  Config().m_maxPaths = val;      
}

void GeneralPage::matchingToleranceSpinBoxValueChanged(int val)
{
  Config().m_matchingTolerance = val;     
}

void GeneralPage::briefStateChanged(int)
{
  Config().m_brief = brief->isChecked();
}

void GeneralPage::emulatedExitsStateChanged(int)
{
  Config().m_emulatedExits = emulatedExits->isChecked();
}


void GeneralPage::updatedStateChanged(int)
{
  Config().m_showUpdated = updated->isChecked();
}



void GeneralPage::drawNotMappedExitsStateChanged(int)
{
  Config().m_drawNotMappedExits = drawNotMappedExits->isChecked();
}

void GeneralPage::drawUpperLayersTexturedStateChanged(int)
{
  Config().m_drawUpperLayersTextured = drawUpperLayersTextured->isChecked();      
}

void GeneralPage::autoLoadFileNameTextChanged(const QString&)
{
  Config().m_autoLoadFileName = autoLoadFileName->text();
}

void GeneralPage::autoLoadCheckStateChanged(int)
{
  Config().m_autoLoadWorld = autoLoadCheck->isChecked(); 
}

void GeneralPage::logFileNameTextChanged(const QString&)
{
  Config().m_logFileName = logFileName->text();
}

void GeneralPage::logCheckStateChanged(int)
{
  Config().m_autoLog = logCheck->isChecked(); 
}

