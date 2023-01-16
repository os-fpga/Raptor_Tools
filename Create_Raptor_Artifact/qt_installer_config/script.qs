function Controller()
{
}

Controller.prototype.FinishedPageCallback = function()
{
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
        widget.MessageLabel.setText("Raptor Installation Completed\n\n\tRaptor is installed at below path\n"+ "\n" + installer.value("TargetDir") + "/RapidSilicon/Rapotr/" + installer.value("Version") + "\n");
    }
}
