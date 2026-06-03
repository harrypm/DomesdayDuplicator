/************************************************************************

    configurationdialog.cpp

    Capture application for the Domesday Duplicator
    DomesdayDuplicator - LaserDisc RF sampler
    Copyright (C) 2018-2019 Simon Inns

    This file is part of Domesday Duplicator.

    Domesday Duplicator is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Email: simon.inns@gmail.com

************************************************************************/

#include "configurationdialog.h"
#include "ui_configurationdialog.h"
#include "AudioResampler.h"
#include <QProcess>
#include <QFile>
#include <QDebug>

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.reset(new Ui::ConfigurationDialog());
    ui->setupUi(this);

    // Build the captureFormatComboBox
    ui->captureFormatComboBox->clear();
    ui->captureFormatComboBox->addItem("FLAC", Configuration::CaptureFormat::flacDirect);
    ui->captureFormatComboBox->addItem("16-bit Signed Raw", Configuration::CaptureFormat::sixteenBitSigned);
    ui->captureFormatComboBox->addItem("10-bit Packed Unsigned", Configuration::CaptureFormat::tenBitPacked);
    
    // sampleRateComboBox is populated dynamically in onCaptureFormatChanged()
    ui->sampleRateComboBox->clear();
    
    // Build the flacOutputFormatComboBox
    ui->flacOutputFormatComboBox->clear();
    ui->flacOutputFormatComboBox->addItem(".flac - Direct FLAC (8-bit)", 0);
    ui->flacOutputFormatComboBox->addItem(".flac - Direct FLAC (16-bit)", 1);
    ui->flacOutputFormatComboBox->addItem(".ldf - ld-compress style", 2);

    // Build the flacCompressionLevelComboBox
    ui->flacCompressionLevelComboBox->clear();
    ui->flacCompressionLevelComboBox->addItem("0 - Fast (largest files)", 0);
    ui->flacCompressionLevelComboBox->addItem("1 - Fast", 1);
    ui->flacCompressionLevelComboBox->addItem("2 - Fast", 2);
    ui->flacCompressionLevelComboBox->addItem("3 - Fast", 3);
    ui->flacCompressionLevelComboBox->addItem("4 - Fast", 4);
    ui->flacCompressionLevelComboBox->addItem("5 - Default (balanced)", 5);
    ui->flacCompressionLevelComboBox->addItem("6 - High", 6);
    ui->flacCompressionLevelComboBox->addItem("7 - High", 7);
    ui->flacCompressionLevelComboBox->addItem("8 - Best (smallest files, slowest)", 8);

    // Build the diskBufferQueueSizeComboBox
    ui->diskBufferQueueSizeComboBox->clear();
    ui->diskBufferQueueSizeComboBox->addItem("64MB", 64 * 1024 * 1024);
    ui->diskBufferQueueSizeComboBox->addItem("128MB", 128 * 1024 * 1024);
    ui->diskBufferQueueSizeComboBox->addItem("256MB", 256 * 1024 * 1024);
    ui->diskBufferQueueSizeComboBox->addItem("512MB", 512 * 1024 * 1024);

    // Build the serialSpeedComboBox
    ui->serialSpeedComboBox->clear();
    ui->serialSpeedComboBox->addItem("Auto", Configuration::SerialSpeeds::autoDetect);
    ui->serialSpeedComboBox->addItem("9600", Configuration::SerialSpeeds::bps9600);
    ui->serialSpeedComboBox->addItem("4800", Configuration::SerialSpeeds::bps4800);
    ui->serialSpeedComboBox->addItem("2400", Configuration::SerialSpeeds::bps2400);
    ui->serialSpeedComboBox->addItem("1200", Configuration::SerialSpeeds::bps1200);
    
    // Build the themeComboBox
    ui->themeComboBox->clear();
    ui->themeComboBox->addItem("Auto (System Default)", 0);
    ui->themeComboBox->addItem("Light Theme", 1);
    ui->themeComboBox->addItem("Dark Theme", 2);

    // If we're running on Linux, disable Windows-specific options.
#ifndef _WIN32
    ui->useWinUsb->setChecked(false);
    ui->useWinUsb->setEnabled(false);
    ui->useAsyncFileIo->setChecked(false);
    ui->useAsyncFileIo->setEnabled(false);
#endif

    // Connect signals for capture format and sample rate selection to update control visibility
    connect(ui->captureFormatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConfigurationDialog::onCaptureFormatChanged);
    connect(ui->sampleRateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConfigurationDialog::onSampleRateChanged);
}

ConfigurationDialog::~ConfigurationDialog()
{
}

void ConfigurationDialog::updateDeviceList(const std::vector<std::string>& deviceList)
{
    // Build the captureFormatComboBox
    ui->preferredDeviceComboBox->clear();
    for (const auto& devicePath : deviceList)
    {
        ui->preferredDeviceComboBox->addItem(devicePath.c_str(), devicePath.c_str());
    }
}

// Load the configuration settings into the UI widgets
void ConfigurationDialog::loadConfiguration(const Configuration& configuration)
{
    // Read the configuration and set up the widgets

    // Capture
    ui->captureDirectoryLineEdit->setText(configuration.getCaptureDirectory());
    
    // Handle capture format and sample rate loading
    Configuration::CaptureFormat configFormat = configuration.getCaptureFormat();
    int storedSampleRateKHz = configuration.getSampleRate(); // kHz value

    // Map combined raw formats to base format (sample rate already stored as kHz)
    if (configFormat == Configuration::CaptureFormat::sixteenBitSigned_Half) {
        ui->captureFormatComboBox->setCurrentIndex(ui->captureFormatComboBox->findData(static_cast<unsigned int>(Configuration::CaptureFormat::sixteenBitSigned)));
        storedSampleRateKHz = 20000;
    } else if (configFormat == Configuration::CaptureFormat::sixteenBitSigned_Quarter) {
        ui->captureFormatComboBox->setCurrentIndex(ui->captureFormatComboBox->findData(static_cast<unsigned int>(Configuration::CaptureFormat::sixteenBitSigned)));
        storedSampleRateKHz = 10000;
    } else if (configFormat == Configuration::CaptureFormat::ldfCompressed) {
        // LDF is chosen via FLAC output format drop-down.
        ui->captureFormatComboBox->setCurrentIndex(ui->captureFormatComboBox->findData(static_cast<unsigned int>(Configuration::CaptureFormat::flacDirect)));
    } else {
        ui->captureFormatComboBox->setCurrentIndex(ui->captureFormatComboBox->findData(static_cast<unsigned int>(configFormat)));
    }

    // onCaptureFormatChanged() already rebuilt the combo — find item by kHz data value
    int sampleRateItemIndex = ui->sampleRateComboBox->findData(storedSampleRateKHz);
    if (sampleRateItemIndex < 0) sampleRateItemIndex = 0;  // fallback to first item
    ui->sampleRateComboBox->setCurrentIndex(sampleRateItemIndex);
    ui->flacCompressionLevelComboBox->setCurrentIndex(ui->flacCompressionLevelComboBox->findData(configuration.getFlacCompressionLevel()));
    int flacOutputFormat = configuration.getFlacOutputFormat();
    if (configFormat == Configuration::CaptureFormat::ldfCompressed) {
        // Legacy configs used index 1 for LDF; index 2 is used in newer builds.
        flacOutputFormat = 2;
    }
    if (flacOutputFormat < 0 || flacOutputFormat > 2) {
        flacOutputFormat = 0;
    }
    ui->flacOutputFormatComboBox->setCurrentIndex(flacOutputFormat);

    // USB
    ui->vendorIdLineEdit->setText(QString::number(configuration.getUsbVid()));
    ui->productIdLineEdit->setText(QString::number(configuration.getUsbPid()));
    ui->preferredDeviceComboBox->setCurrentText(configuration.getUsbPreferredDevice());
    ui->diskBufferQueueSizeComboBox->setCurrentIndex(ui->diskBufferQueueSizeComboBox->findData((qulonglong)configuration.getDiskBufferQueueSize()));
    ui->useSmallUsbTransferQueue->setChecked(configuration.getUseSmallUsbTransferQueue());
    ui->useSmallUsbTransfers->setChecked(configuration.getUseSmallUsbTransfers());
#ifdef _WIN32
    ui->useWinUsb->setChecked(configuration.getUseWinUsb());
    ui->useAsyncFileIo->setChecked(configuration.getUseAsyncFileIo());
#endif

    // Player Integration

    // Build the serialDeviceComboBox
    ui->serialDeviceComboBox->clear();
    const auto infos = QSerialPortInfo::availablePorts();

    // Add additional "None" option to allow de-selection of COM port
    ui->serialDeviceComboBox->addItem(QString(tr("None")), QString(tr("None")));

    bool configuredSerialDevicePresent = false;
    for (const QSerialPortInfo &info : infos) {
        ui->serialDeviceComboBox->addItem(info.portName(), info.portName());

        // Is this the currently configured serial device?
        if (info.portName() == configuration.getSerialDevice())
                configuredSerialDevicePresent = true;
    }

    // Select the currently configured device (or default to 'none' if the device is not set)
    if (!configuredSerialDevicePresent) {
        // No device is present in the configuration or the configured device is no longer available - set to none
        ui->serialDeviceComboBox->setCurrentIndex(0);
    } else {
        // Set to the configured device
        int index = ui->serialDeviceComboBox->findData(configuration.getSerialDevice());
        ui->serialDeviceComboBox->setCurrentIndex(index);
    }

    // Select the currently configured serial speed
    ui->serialSpeedComboBox->setCurrentIndex(ui->serialSpeedComboBox->findData(static_cast<unsigned int>(configuration.getSerialSpeed())));

    // Keylock flag
    ui->keyLockCheckBox->setChecked(configuration.getKeyLock());

    // Advanced naming
    ui->perSideNotesCheckBox->setChecked(configuration.getPerSideNotesEnabled());
    ui->perSideMintCheckBox->setChecked(configuration.getPerSideMintEnabled());

    // Amplitude
    ui->amplitudeLabelCheckBox->setChecked(configuration.getAmplitudeLabelEnabled());
    ui->amplitudeChartCheckBox->setChecked(configuration.getAmplitudeChartEnabled());
    
    // Theme
    ui->themeComboBox->setCurrentIndex(configuration.getThemeStyle());
    
    // Update FLAC control visibility based on selected format
    onCaptureFormatChanged(ui->captureFormatComboBox->currentIndex());
}

// Save the configuration settings from the UI widgets
void ConfigurationDialog::saveConfiguration(Configuration& configuration)
{
    qDebug() << "ConfigurationDialog::saveConfiguration(): Saving configuration";

    // Capture
    configuration.setCaptureDirectory(ui->captureDirectoryLineEdit->text());
    
    // Combine capture format and sample rate into final format
    Configuration::CaptureFormat baseFormat = static_cast<Configuration::CaptureFormat>(ui->captureFormatComboBox->itemData(ui->captureFormatComboBox->currentIndex()).toInt());
    int sampleRateKHz = ui->sampleRateComboBox->currentData().toInt(); // stored as kHz
    int flacOutputFormat = ui->flacOutputFormatComboBox->currentIndex();

    Configuration::CaptureFormat finalFormat = baseFormat;

    // For 16-bit raw: encode sample rate into the format enum (backward compat)
    if (baseFormat == Configuration::CaptureFormat::sixteenBitSigned) {
        if (sampleRateKHz == 20000) {
            finalFormat = Configuration::CaptureFormat::sixteenBitSigned_Half;
        } else if (sampleRateKHz == 10000) {
            finalFormat = Configuration::CaptureFormat::sixteenBitSigned_Quarter;
        }
        // else keep as sixteenBitSigned (40 MSPS full rate)
    }
    // For FLAC: choose ldfCompressed vs flacDirect based on output format dropdown
    else if (baseFormat == Configuration::CaptureFormat::flacDirect) {
        if (flacOutputFormat == 2) {
            finalFormat = Configuration::CaptureFormat::ldfCompressed;
        }
        // else stay as flacDirect; sample rate stored separately as kHz
    }

    configuration.setCaptureFormat(finalFormat);
    configuration.setFlacCompressionLevel(ui->flacCompressionLevelComboBox->itemData(ui->flacCompressionLevelComboBox->currentIndex()).toInt());
    configuration.setFlacOutputFormat(ui->flacOutputFormatComboBox->currentIndex());
    configuration.setSampleRate(sampleRateKHz); // store actual kHz

    // USB
    configuration.setUsbVid(static_cast<quint16>(ui->vendorIdLineEdit->text().toInt()));
    configuration.setUsbPid(static_cast<quint16>(ui->productIdLineEdit->text().toInt()));
    configuration.setUsbPreferredDevice(ui->preferredDeviceComboBox->currentText());
    configuration.setDiskBufferQueueSize((size_t)ui->diskBufferQueueSizeComboBox->itemData(ui->diskBufferQueueSizeComboBox->currentIndex()).toULongLong());
    configuration.setUseSmallUsbTransferQueue(ui->useSmallUsbTransferQueue->isChecked());
    configuration.setUseSmallUsbTransfers(ui->useSmallUsbTransfers->isChecked());
    configuration.setUseWinUsb(ui->useWinUsb->isChecked());
    configuration.setUseAsyncFileIo(ui->useAsyncFileIo->isChecked());

    // Player integration - serial device
    configuration.setSerialDevice(ui->serialDeviceComboBox->currentText());

    // Player integration - Serial speed
    configuration.setSerialSpeed(static_cast<Configuration::SerialSpeeds>(ui->serialSpeedComboBox->itemData(ui->serialSpeedComboBox->currentIndex()).toInt()));

    // KeyLock
    configuration.setKeyLock(ui->keyLockCheckBox->isChecked());

    // Advanced naming
    configuration.setPerSideNotesEnabled(ui->perSideNotesCheckBox->isChecked());
    configuration.setPerSideMintEnabled(ui->perSideMintCheckBox->isChecked());

    // Amplitude
    configuration.setAmplitudeLabelEnabled(ui->amplitudeLabelCheckBox->isChecked());
    configuration.setAmplitudeChartEnabled(ui->amplitudeChartCheckBox->isChecked());
    
    // Theme
    configuration.setThemeStyle(ui->themeComboBox->currentIndex());

    // Save the configuration to disk
    configuration.writeConfiguration();
}

// Browse for capture directory button clicked
void ConfigurationDialog::on_captureDirectoryPushButton_clicked()
{
    QString captureDirectoryPath;

    captureDirectoryPath = QFileDialog::getExistingDirectory(this, tr("Select capture directory"), ui->captureDirectoryLineEdit->text());

    if (captureDirectoryPath.isEmpty()) {
        qDebug() << "ConfigurationDialog::on_captureDirectoryPushButton_clicked(): QFileDialog::getExistingDirectory returned empty directory path";
    } else {
        ui->captureDirectoryLineEdit->setText(captureDirectoryPath);
    }
}

// Save configuration clicked
void ConfigurationDialog::on_buttonBox_accepted()
{
    qDebug() << "ConfigurationDialog::on_buttonBox_accepted(): Configuration changed";

    // Emit a configuration changed signal
    emit configurationChanged();
}

// Cancel configuration clicked
void ConfigurationDialog::on_buttonBox_rejected()
{
    qDebug() << "ConfigurationDialog::on_buttonBox_rejected(): Ignoring configuration changes";
}

// Any button clicked
void ConfigurationDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    // Check for restore defaults button
    if (button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        qDebug() << "ConfigurationDialog::on_buttonBox_clicked(): Restore defaults clicked";

        Configuration defaultConfig(this);
        defaultConfig.setDefault();
        loadConfiguration(defaultConfig);
    }
}

void ConfigurationDialog::encodeToFlac(const QString& inputFilePath, const QString& outputFilePath)
{
    QProcess flacProcess;

    // Set up the FLAC encoder command
    QStringList arguments;
    arguments << "-8"  // Compression level 8
              << "--endian=little"  // Little-endian input
              << "--sign=signed"  // Signed input
              << "--channels=1"  // Mono
              << "--bps=16"  // 16 bits per sample
              << "--sample-rate=40000"  // Sample rate
              << "--threads=8"  // Default to 8 threads
              << "-o" << outputFilePath  // Output file
              << "-";  // Read from stdin

    flacProcess.setProgram("flac");
    flacProcess.setArguments(arguments);

    // Open the input file
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input file:" << inputFilePath;
        return;
    }

    // Start the FLAC process
    flacProcess.start();

    // Write the input file data to the FLAC process
    while (!inputFile.atEnd()) {
        QByteArray buffer = inputFile.read(4096);
        flacProcess.write(buffer);
    }

    inputFile.close();
    flacProcess.closeWriteChannel();

    // Wait for the process to finish
    if (!flacProcess.waitForFinished()) {
        qWarning() << "FLAC encoding process failed:" << flacProcess.errorString();
        return;
    }

    qDebug() << "FLAC encoding completed successfully. Output file:" << outputFilePath;
}

void ConfigurationDialog::encodeToLdf(const QString& inputFilePath, const QString& outputFilePath, int downsampleFactor)
{
    QProcess flacProcess;

    // Calculate the effective sample rate after downsampling
    int baseSampleRate = 40000;  // 40 kHz base sample rate
    int effectiveSampleRate = baseSampleRate / downsampleFactor;

    // Set up the FLAC encoder command for LDF format (ld-compress compatible)
    // LDF uses FLAC compression with specific settings to match ld-compress output
    QStringList arguments;
    arguments << "-8"  // Use level 8 for LDF (maximum compression for ld-compress compatibility)
              << "--endian=little"  // Little-endian input
              << "--sign=signed"  // Signed input
              << "--channels=1"  // Mono
              << "--bps=16"  // 16 bits per sample
              << QString("--sample-rate=%1").arg(effectiveSampleRate)  // Correct sample rate for downsampled data
              << "--exhaustive-model-search"  // Better compression matching ld-compress
              << "--qlp-coeff-precision-search"  // Optimize coefficient precision
              << "--threads=8"  // Multi-threading support
              << "-o" << outputFilePath  // Output file
              << "-";  // Read from stdin

    flacProcess.setProgram("flac");
    flacProcess.setArguments(arguments);

    // Open the input file
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input file for LDF encoding:" << inputFilePath;
        return;
    }

    qDebug() << "Starting LDF (ld-compress compatible) encoding with" << downsampleFactor << "x downsampling...";

    // Initialize resampler if downsampling is required
    std::unique_ptr<AudioResampler> resampler;
    if (downsampleFactor > 1) {
        resampler = std::make_unique<AudioResampler>();
        if (!resampler->initialize(baseSampleRate, effectiveSampleRate)) {
            qWarning() << "Failed to initialize audio resampler";
            inputFile.close();
            return;
        }
    }

    // Start the FLAC process
    flacProcess.start();
    if (!flacProcess.waitForStarted()) {
        qWarning() << "Failed to start FLAC process for LDF encoding:" << flacProcess.errorString();
        inputFile.close();
        return;
    }

    // Process the input file data with resampling if needed
    const qint64 inputBufferSize = 32768;  // Input buffer size (16-bit samples)
    std::vector<int16_t> inputBuffer(inputBufferSize / 2);  // Buffer for 16-bit samples
    std::vector<int16_t> outputBuffer;
    
    if (resampler) {
        // Pre-allocate output buffer for resampling
        int expectedOutputSize = resampler->getExpectedOutputSampleCount(inputBufferSize / 2);
        outputBuffer.resize(expectedOutputSize + 1024); // Add some extra space for safety
    }

    while (!inputFile.atEnd()) {
        QByteArray rawData = inputFile.read(inputBufferSize);
        if (rawData.isEmpty()) break;
        
        // Convert QByteArray to 16-bit signed samples
        int sampleCount = rawData.size() / 2;
        memcpy(inputBuffer.data(), rawData.data(), rawData.size());
        
        QByteArray outputData;
        
        if (resampler) {
            // Perform resampling
            int resampledSamples = resampler->resample(inputBuffer.data(), sampleCount, 
                                                      outputBuffer.data(), outputBuffer.size());
            if (resampledSamples < 0) {
                qWarning() << "Resampling failed";
                break;
            }
            
            // Convert resampled data back to QByteArray
            outputData = QByteArray((const char*)outputBuffer.data(), resampledSamples * 2);
        } else {
            // No resampling needed, use original data
            outputData = rawData;
        }
        
        // Write to FLAC process
        qint64 written = flacProcess.write(outputData);
        if (written != outputData.size()) {
            qWarning() << "Failed to write complete buffer to FLAC process";
            break;
        }
        
        // Allow for real-time processing
        QCoreApplication::processEvents();
    }

    inputFile.close();
    flacProcess.closeWriteChannel();

    // Wait for the process to finish with timeout
    if (!flacProcess.waitForFinished(300000)) {  // 5 minute timeout
        qWarning() << "LDF encoding process timed out or failed:" << flacProcess.errorString();
        flacProcess.kill();
        return;
    }

    if (flacProcess.exitCode() != 0) {
        qWarning() << "LDF encoding process failed with exit code:" << flacProcess.exitCode();
        qWarning() << "Standard error:" << flacProcess.readAllStandardError();
        return;
    }

    qDebug() << "LDF encoding completed successfully. Output file:" << outputFilePath;
}

void ConfigurationDialog::encodeToFlacDirect(const QString& inputFilePath, const QString& outputFilePath, int compressionLevel, int downsampleFactor)
{
    QProcess flacProcess;

    // Calculate the effective sample rate after downsampling
    int baseSampleRate = 40000;  // 40 kHz base sample rate
    int effectiveSampleRate = baseSampleRate / downsampleFactor;

    // Set up the FLAC encoder command for direct FLAC output (like misrc_capture)
    // Direct FLAC uses standard settings optimized for speed and compatibility
    QStringList arguments;
    arguments << QString("-%1").arg(compressionLevel)  // Use configurable compression level
              << "--endian=little"  // Little-endian input
              << "--sign=signed"  // Signed input
              << "--channels=1"  // Mono
              << "--bps=16"  // 16 bits per sample
              << QString("--sample-rate=%1").arg(effectiveSampleRate)  // Correct sample rate for downsampled data
              << "--threads=8"  // Multi-threading support
              << "--no-seektable"  // Skip seektable for streaming compatibility
              << "-o" << outputFilePath  // Output file
              << "-";  // Read from stdin

    flacProcess.setProgram("flac");
    flacProcess.setArguments(arguments);

    // Open the input file
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input file for direct FLAC encoding:" << inputFilePath;
        return;
    }

    qDebug() << "Starting direct FLAC encoding with" << downsampleFactor << "x downsampling...";

    // Initialize resampler if downsampling is required
    std::unique_ptr<AudioResampler> resampler;
    if (downsampleFactor > 1) {
        resampler = std::make_unique<AudioResampler>();
        if (!resampler->initialize(baseSampleRate, effectiveSampleRate)) {
            qWarning() << "Failed to initialize audio resampler";
            inputFile.close();
            return;
        }
    }

    // Start the FLAC process
    flacProcess.start();
    if (!flacProcess.waitForStarted()) {
        qWarning() << "Failed to start FLAC process for direct encoding:" << flacProcess.errorString();
        inputFile.close();
        return;
    }

    // Process the input file data with resampling if needed
    const qint64 inputBufferSize = 32768;  // Input buffer size (16-bit samples)
    std::vector<int16_t> inputBuffer(inputBufferSize / 2);  // Buffer for 16-bit samples
    std::vector<int16_t> outputBuffer;
    
    if (resampler) {
        // Pre-allocate output buffer for resampling
        int expectedOutputSize = resampler->getExpectedOutputSampleCount(inputBufferSize / 2);
        outputBuffer.resize(expectedOutputSize + 1024); // Add some extra space for safety
    }

    while (!inputFile.atEnd()) {
        QByteArray rawData = inputFile.read(inputBufferSize);
        if (rawData.isEmpty()) break;
        
        // Convert QByteArray to 16-bit signed samples
        int sampleCount = rawData.size() / 2;
        memcpy(inputBuffer.data(), rawData.data(), rawData.size());
        
        QByteArray outputData;
        
        if (resampler) {
            // Perform resampling
            int resampledSamples = resampler->resample(inputBuffer.data(), sampleCount, 
                                                      outputBuffer.data(), outputBuffer.size());
            if (resampledSamples < 0) {
                qWarning() << "Resampling failed";
                break;
            }
            
            // Convert resampled data back to QByteArray
            outputData = QByteArray((const char*)outputBuffer.data(), resampledSamples * 2);
        } else {
            // No resampling needed, use original data
            outputData = rawData;
        }
        
        // Write to FLAC process
        qint64 written = flacProcess.write(outputData);
        if (written != outputData.size()) {
            qWarning() << "Failed to write complete buffer to FLAC process";
            break;
        }
        
        // Allow for real-time processing
        QCoreApplication::processEvents();
    }

    inputFile.close();
    flacProcess.closeWriteChannel();

    // Wait for the process to finish with reasonable timeout
    if (!flacProcess.waitForFinished(180000)) {  // 3 minute timeout
        qWarning() << "Direct FLAC encoding process timed out or failed:" << flacProcess.errorString();
        flacProcess.kill();
        return;
    }

    if (flacProcess.exitCode() != 0) {
        qWarning() << "Direct FLAC encoding process failed with exit code:" << flacProcess.exitCode();
        qWarning() << "Standard error:" << flacProcess.readAllStandardError();
        return;
    }

    qDebug() << "FLAC Direct encoding completed successfully. Output file:" << outputFilePath;
}

void ConfigurationDialog::downsampleRawFile(const QString& inputFilePath, const QString& outputFilePath, int downsampleFactor)
{
    // Open the input file
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input file for downsampling:" << inputFilePath;
        return;
    }
    
    // Open the output file
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open output file for downsampling:" << outputFilePath;
        inputFile.close();
        return;
    }
    
    qDebug() << "Starting raw file downsampling with factor" << downsampleFactor;
    
    // Initialize resampler
    AudioResampler resampler;
    int baseSampleRate = 40000;  // 40 kHz base sample rate
    int effectiveSampleRate = baseSampleRate / downsampleFactor;
    
    if (!resampler.initialize(baseSampleRate, effectiveSampleRate)) {
        qWarning() << "Failed to initialize audio resampler for raw file downsampling";
        inputFile.close();
        outputFile.close();
        return;
    }
    
    // Process the file in chunks
    const qint64 inputBufferSize = 32768;  // Input buffer size (16-bit samples)
    std::vector<int16_t> inputBuffer(inputBufferSize / 2);  // Buffer for 16-bit samples
    std::vector<int16_t> outputBuffer;
    
    // Pre-allocate output buffer for resampling
    int expectedOutputSize = resampler.getExpectedOutputSampleCount(inputBufferSize / 2);
    outputBuffer.resize(expectedOutputSize + 1024); // Add some extra space for safety
    
    while (!inputFile.atEnd()) {
        QByteArray rawData = inputFile.read(inputBufferSize);
        if (rawData.isEmpty()) break;
        
        // Convert QByteArray to 16-bit signed samples
        int sampleCount = rawData.size() / 2;
        memcpy(inputBuffer.data(), rawData.data(), rawData.size());
        
        // Perform resampling
        int resampledSamples = resampler.resample(inputBuffer.data(), sampleCount, 
                                                 outputBuffer.data(), outputBuffer.size());
        if (resampledSamples < 0) {
            qWarning() << "Resampling failed during raw file processing";
            break;
        }
        
        // Convert resampled data back to QByteArray and write to output
        QByteArray outputData((const char*)outputBuffer.data(), resampledSamples * 2);
        qint64 written = outputFile.write(outputData);
        if (written != outputData.size()) {
            qWarning() << "Failed to write complete buffer to output file";
            break;
        }
    }
    
    inputFile.close();
    outputFile.close();
    
    qDebug() << "Raw file downsampling completed successfully. Output file:" << outputFilePath;
}

void ConfigurationDialog::onCaptureFormatChanged(int index)
{
    // Get the selected capture format
    Configuration::CaptureFormat selectedFormat = static_cast<Configuration::CaptureFormat>(
        ui->captureFormatComboBox->itemData(index).toInt());
    
    // Show FLAC-related controls only for FLAC format
    bool showFlacControls = (selectedFormat == Configuration::CaptureFormat::flacDirect);

    ui->flacCompressionLabel->setVisible(showFlacControls);
    ui->flacCompressionLevelComboBox->setVisible(showFlacControls);
    ui->flacOutputFormatLabel->setVisible(showFlacControls);
    ui->flacOutputFormatComboBox->setVisible(showFlacControls);

    // Show sample rate control for 16-bit formats (both raw and FLAC)
    bool showSampleRateControls = (selectedFormat == Configuration::CaptureFormat::sixteenBitSigned ||
                                   selectedFormat == Configuration::CaptureFormat::flacDirect);
    ui->sampleRateLabel->setVisible(showSampleRateControls);
    ui->sampleRateComboBox->setVisible(showSampleRateControls);

    if (!showSampleRateControls)
        return;

    // Rebuild sample rate combo with format-appropriate options (stored as kHz)
    int prevKHz = ui->sampleRateComboBox->currentData().toInt();
    ui->sampleRateComboBox->blockSignals(true);
    ui->sampleRateComboBox->clear();

    if (selectedFormat == Configuration::CaptureFormat::flacDirect) {
        // Native FLAC path supports arbitrary target rates via software resampling.
        ui->sampleRateComboBox->addItem("40 MSPS",                40000);
        ui->sampleRateComboBox->addItem("28 MSPS",                28000);
        ui->sampleRateComboBox->addItem("24 MSPS (S-VHS/Video8)", 24000);
        ui->sampleRateComboBox->addItem("20 MSPS (recommended)",  20000);
        ui->sampleRateComboBox->addItem("18 MSPS",                18000);
        ui->sampleRateComboBox->addItem("16 MSPS",                16000);
        ui->sampleRateComboBox->addItem("10 MSPS",                10000);
    } else {
        // Raw 16-bit — software downsampling supports integer fractions only
        ui->sampleRateComboBox->addItem("40 MSPS (Full Rate)", 40000);
        ui->sampleRateComboBox->addItem("20 MSPS (1/2 Rate)", 20000);
        ui->sampleRateComboBox->addItem("10 MSPS (1/4 Rate)", 10000);
    }

    // Restore previous selection if available, else default to first item
    int idx = ui->sampleRateComboBox->findData(prevKHz);
    ui->sampleRateComboBox->setCurrentIndex(idx >= 0 ? idx : 0);
    ui->sampleRateComboBox->blockSignals(false);
}

void ConfigurationDialog::onSampleRateChanged(int index)
{
    // This method can be used for any future sample rate-specific logic
    Q_UNUSED(index);
}
