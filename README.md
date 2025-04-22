F429741
Marlan Richards

This project presents the implementation of an intelligent temperature monitoring system on an Arduino platform. The system dynamically adjusts both its sampling rate and power mode based on the frequency and magnitude of temperature fluctuations. The primary goal is to achieve a balance between data accuracy and energy efficiency, using signal processing and predictive analytics techniques.

By collecting real-time data, analyzing it through a Discrete Fourier Transform (DFT), and predicting future behavior using a moving average, the system decides when to remain in Active Mode or transition into lower power modes such as Idle or Power Down.

Here’s a revised version of the README content in first-person, omitting the code:

List of additions:

24WSA024_JNB_Arduino_Coursework_Final_2025S2(V1).ipynb
Created using Colab

CMakeLists.txt
c coursework

Heat_check.ino.ino
Update Heat_check.ino.ino

Marlan_Arduino_setup.JPG
Add files via upload

README.md
Create README.md

Task_4_Temperature_Optimization_Report_Final.ipynb
Created using Colab

main.c
c coursework

threeminstemperature_F429741.csv
Add files via upload
⸻

Git and Arduino Tasks

Task 1: Version Control

Task 1.1: Authentication Issues in Google Colab

If my private GitHub repository isn’t appearing in Google Colab when trying to save a notebook, it’s likely due to authentication issues. Colab accesses GitHub repositories via OAuth, so I need to make sure I’m signed in properly and have authorized Google Colab on GitHub. If it still doesn’t show, I need to double-check the repository permissions to ensure my account has access.

Once the authentication is successful, I can access private repositories through the GitHub section in Colab’s settings.

Task 1.2: GitHub Action for Automated Task Tracking

To automate tracking the completion of tasks 1.3 and 1.4, I’ve set up a GitHub Action workflow. This workflow verifies two things:
	•	If the test-branch was deleted.
	•	If a merge conflict in project-notes.md was resolved.

I just need to add the workflow file .github/workflows/task_verification.yml to my repository and push the changes.

Task 1.3: Unable to Delete a Branch

If I try to delete a branch with git branch -d test-branch and it fails, it could be because there are unmerged changes. To forcefully delete the branch, I can use git branch -D test-branch after making sure no important work is lost.

Task 1.4: Handling a Merge Conflict

When I merge two branches with conflicting changes in project-notes.md, Git will flag a merge conflict. To resolve it, I edit the file manually, mark the conflict as resolved with git add project-notes.md, and then commit the changes with git commit -m "Resolved merge conflict".

This will be automatically verified in Task 1.2 through the GitHub Action I set up.

Task 2: Arduino Temperature Sensor Data Collection

Task 2.1: Grove Temperature Sensor

I’m using the Grove Temperature Sensor (V1.2), which measures temperature through a thermistor. In the code, I read the analog input and use the sensor’s formula to convert the data into temperature readings.

Task 3: Implementing Functions in Arduino IDE

Task 3.1: Data Collection Function

I’ve written a function to collect temperature data over a 3-minute period. It saves the readings in an array, ensuring each temperature value is captured every few milliseconds.

Task 3.2: Apply DFT for Frequency Analysis

I apply the Discrete Fourier Transform (DFT) to the collected temperature data to analyze the frequency components. This helps identify the dominant frequency of temperature variation.

Task 3.3: Send Data to PC

After collecting the data, I send the temperature readings along with timestamps to my PC for further analysis, using serial communication.

Task 3.4: Power Mode Decision

Based on the observed temperature variation, I decide the power mode of the system. If the variation is high, the system stays active; if it’s moderate, it goes into idle mode, and if it’s low, it enters power-down mode to conserve energy.

Task 3.5: Save Data to CSV

I save the temperature data and frequency analysis results in a .csv file. This makes it easier for me to analyze the data later or share it for reporting.

Task 3.6: Data Visualization

I use Python to plot graphs of the temperature data over time and the frequency-domain components. This helps me visually analyze the behavior of the system.

Task 4: Adaptive Sampling Rate Optimization

This task focuses on optimizing the system’s sampling rate based on temperature variations. I’ve set up the system to automatically adjust the sampling rate and power mode:
	•	Active Mode: High sampling rate for rapid data collection.
	•	Idle Mode: Medium sampling rate for less frequent data collection.
	•	Power-Down Mode: Low sampling rate to save energy.

The system dynamically adjusts the sampling rate based on the frequency of temperature fluctuations, ensuring efficient data collection and energy management.
