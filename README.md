#Instron Parser - The Barret Data Toolbox
##Description
This program is designed to take a set of files that the Bluehill control software for an Instron load frame will output when configured to export raw data.
##Usage
It takes this data, plots it and finds the minimum value in the range 5mm - 'End of Data'. You may then define a custom range for each sample by clicking on both min and max points. Data can be copied to the clipboard by clicking 'Copy Data to Clipboard', selecting the data is not necessary as it will automatically copy all data. Copying a portion of the data is not possible at this time.
##Screenshot
![Appearance on First Opening](http://i.imgur.com/LOWUt.png)
##Program Information
This was written in C++ using the Qt and Qwt libraries.