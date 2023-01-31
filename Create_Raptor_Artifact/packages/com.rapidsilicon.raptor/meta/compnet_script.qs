/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
    if (!installer.isCommandLineInstance())
        gui.pageWidgetByObjectName("LicenseAgreementPage").entered.connect(changeLicenseLabels);

}

Component.prototype.createOperations = function()
{

    component.createOperations();

    var result = QMessageBox["question"]("Raptor.Dependencies", "Install Raptor OS Dependencies", "Do you want installer do the OS dependecies installation?<br>" +
        "This needs SUDO or Admin previlages.", QMessageBox.Ok | QMessageBox.No);

    try {
        if (result == QMessageBox.Ok && systemInfo.currentCpuArchitecture === "x86_64")  {
            component.addOperation("Execute", "workingDirectory=@TargetDir@", "errormessage=Need SUDO but can't find it. Either Click Cancel to invoke installer again with SUDO or ignore to let the installer install Raptor only and later connect support@rapidsilicon.com with your OS details to get list of OS related dependencies.", "bash", "Raptor/" + installer.value("Version") + "/share/unix_dep.sh");
            } else {
                var dummy;
               // QMessageBox.information("Raptor.Dependencies", "Install from excuter Raptor OS Dependencies", "Raptor requires Visual Studio 2017 Redistributables. Please follow the steps to install it now.", QMessageBox.OK);
            } 
        } catch(e) {
            console.log(e);
        }
}

Component.prototype.installationFinishedPageIsShown = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "ReadMeCheckBoxForm", QInstaller.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }
}   

Component.prototype.installationFinished = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var checkboxForm = component.userInterface( "ReadMeCheckBoxForm" );
            if (checkboxForm && checkboxForm.readMeCheckBox.checked) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/Raptor/" + installer.value("Version") +"/README.md");
            }
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.createOperationsForArchive = function(archive)
{
    component.addOperation("Extract", archive, "@TargetDir@/Raptor/@Version@");
   
}

changeLicenseLabels = function()
{
    page = gui.pageWidgetByObjectName("LicenseAgreementPage");
    page.AcceptLicenseLabel.setText("<b><font color=\"#E0115F\" size=4>Click on ME to Accept above License Agreement</font></b>");
}


