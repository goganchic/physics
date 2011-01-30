#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QDialog>

namespace Ui {
    class ConfigurationWindow;
}

class ConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationWindow(QWidget *parent = 0);
    ~ConfigurationWindow();
    Ui::ConfigurationWindow* getUi() { return ui; }
    int getProcessorBlockSize();
    int getPreprocessorBlockSize();
    int getInterval();

private:
    Ui::ConfigurationWindow *ui;
    void updatePreprocessorBlockSize();
    void updateProcessorBlockSize();
    void updateInterval();

private slots:
    void on_okButton_clicked();
    void on_observationTimeEdit_textEdited(QString );
    void on_deviceBufferSizeEdit_textChanged(QString );
    void on_discretizationRateEdit_textChanged(QString );
    int getX();
};

#endif // CONFIGURATIONWINDOW_H
