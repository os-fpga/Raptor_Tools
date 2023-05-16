function Controller()
{
}

Controller.prototype.IntroductionPageCallback = function()
{
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
        widget.MessageLabel.setText("<center><b><font size=5>Welcome to Raptor Installation Setup</center></font></b>"); // set the welcome text
    }
}

Controller.prototype.FinishedPageCallback = function()
{
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
        widget.MessageLabel.setText("Raptor Installation Completed\n\n\tRaptor is installed at below path\n"+ "\n" + installer.value("TargetDir") + "/Raptor/" + installer.value("Version") + "\n");
    }
}
