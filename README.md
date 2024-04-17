# HOSPITAL_MANAGEMENT_SYSTEM

## Hospital Administrator
•	Login and logout 

•	Register new users 
>> (patient, doctor, admin, nurse) 

•	Appointment trends - Identify peak appointment times and adjust staffing or clinic hours accordingly. 
>>（ Extract RoomNo. from Appointments）

•	Managing inventory (view and search)– (Track hospital supplies and medications, ensuring timely restocking and preventing shortages.) 

## Patient
•	Login and logout 
•	Appointment Management - View, reschedule, and cancel appointments 
•	book slots based on Dr schedule
•	Electronic Health Records (EHR) Access (view only): Secure access to a portion of their medical history, prescriptions, appointment history, and billing information.
•	Billing Management (view and search) - billing details and manage insurance claims.



## Doctor
•	Login and logout 

•	Doctor Scheduler - Manage their schedules and availability for appointments.
>> (Generate based on Appointments TimeSlot)

•	Secure EHR Access (view only) - access to complete patient Electronic Health Records (EHR). 
>>(Use PatientID to query records or CaseName to query similar Case)

Includes: 	- demographics
- medical history,
- allergies
- medications, 
- past procedures
- diagnostic reports. 

•	Reporting and Analytics (view and search) generate reports on their own patients or specific conditions, allowing them to track treatment trends and measure outcomes.
>>(Generate from EHR) 

## Staff Nurse 
•	Login and logout

•	Access doctor schedules and availability for appointmens. 

•	Inventory Management - Access and manage supplies and medications 

•	Reporting and Analytics (view and search) - generate reports on their patients or specific units, allowing them to identify trends and areas for improvement in care delivery. 
>>(Use PatientID to query records or CaseName to query similar Case)

## Commands To Compile Multiple Source Files Together

Turns File into an object
gcc -c "C File"

ex. gcc -c doctor.c -> doctor.o

Compile with linked Object
gcc -o "Name of Executable"  "C File"  "Object"

ex. gcc -o HMS main.c doctor.o admin.o nurse.o doctor.o patient.o -> HMS.exe

When Doing so, Remember to Remove utility.h from the Source Code Before Turning into an Object. (Except the main.c)
