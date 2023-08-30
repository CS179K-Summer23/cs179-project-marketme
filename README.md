[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=11509564&assignment_repo_type=AssignmentRepo)
# Project-CS179 Inventory Management System (IMS) 📦
Team Name: MarketMe (Terminal base tool)

<code><img height="20" src="https://img.shields.io/badge/Powered%20by-C%2B%2B-blue?style=flat-square&logo=c%2B%2B" alt="Powered by C++" title="Powered by C++"></code>
<code><img height="20" src="https://img.shields.io/badge/Database-JSON-orange?style=flat-square&logo=json" alt="JSON" title="JSON"></code>
<code><img height="20" src="https://img.shields.io/badge/Utilizes-UPCitemdb-red?style=flat-square&logo=barcode" alt="Utilizes UPCitemdb" title="Utilizes UPCitemdb"></code>
<code><img height="20" src="https://img.shields.io/badge/Utilizes-GmailAPI-yellow?style=flat-square&logo=gmail" alt="Utilizes Gmail API" title="Utilizes Gmail API"></code>
<code><img height="20" src="https://img.shields.io/badge/Utilizes-cURL%20HTTP%20Request-lightgrey?style=flat-square&logo=curl" alt="cURL HTTP Request" title="cURL HTTP Request"></code>
<code><img height="20" src="https://img.shields.io/badge/Features-Barcode%20Scanner-purple?style=flat-square&logo=barcode" alt="Barcode Scanner" title="Barcode Scanner"></code>
<code><img height="20" src="https://img.shields.io/badge/Features-ID%20Scanner-brown?style=flat-square&logo=id-card" alt="ID Scanner Function" title="ID Scanner Function"></code>
<code><img height="20" src="https://img.shields.io/badge/Design-Singleton%20Pattern-darkgreen?style=flat-square" alt="Uses Singleton Design Pattern" title="Uses Singleton Design Pattern"></code>


## About 📝

MarketMe, a dedicated team of professionals, is enthusiastically working on an **Inventory Management System (IMS)**. Our objective is to develop an effective system to manage your product data, maintain inventories, and track sales, and purchases. Our system aims to provide you with real-time insights, proactive alerts about stock levels, and an exhaustive range of other features that streamline your operations and maximize profitability.

Update: The project is nearing completion. We've successfully completed a code refactoring phase and have implemented the Singleton design pattern. UI enhancements and user experience improvements are currently in progress. While an online database could simplify certain operations, we've opted to continue using JSON for its unique challenges and educational benefits.

## Team 👥

Meet our fantastic team:

- Chun Ho Wong: cwong165
- Henry Pham: hpham096
- Irene Wu: iwu021
- Jason Diep: jdiep018
- Chenlong Li: cli242


## Communications 📡

- Discord: (Our primary channel for daily communications, established on 7/31/2023)
- Weekly Meetings: Every Friday via Zoom (7 PM - 8 PM)

## Technology 📘

Skills Required: 

- Proficiency in C++ programming language
- Knowledge of UPCitemdb, Gmail API via cURL HTTP request
- Familiarity with JSON library and file management
- Understanding of encryption methods
- Database knowledge acquired from the CS166 course or equivalent

**Note:** While we welcome learning and skill development, we wish to focus on perfecting and utilizing our existing skills for efficient software development. 

Programming Environment: Visual Studio Code

## Major features (Software Specifications) ⚙️

MarketMe's IMS is a terminal software offering several essential features (Updated):

1. **UPCitemdb API Integration 🌐**: Facilitate automatic data population by integrating with the UPCitemdb API. This streamlines the addition of new products to the inventory by auto-filling essential attributes, thereby eliminating manual data entry errors and easing the pain of inputting data by hand.

2. **Comprehensive Product Management Suite 🛠️**: Empower administrators with tools for the manual input, update, and deletion of product information. This complete management interface ensures that inventory data remains current and accurate.

3. **Barcode Scanning for Streamlined Product Identification 📸**: Incorporate a barcode scanning feature to simplify and accelerate product identification in various operational tasks such as inventory management and checkout processes.

4. **Gmail API-Driven Alert and Notification System 🔔**: Utilize Gmail API for sending alerts concerning sales fluctuations, low inventory levels, and impending product expiration dates. This system also enables the generation of analytical sales reports for data-driven decision-making.

5. **Subscriber Newsletter with Intelligent Recommendations 🗞️**: Dispatch a regular newsletter that provides subscribers with updates and intelligently recommends clearance items or products nearing their expiration dates, which are offered at discounted rates.

6. **Advanced Checkout and Payment Processing System 💰**: Implement a checkout mechanism capable of dynamically calculating taxes based on zip codes, accepting coupon codes, and updating internal databases. All transaction data is recorded for future analytical uses.

7. **Sales Reporting and Analytical Dashboard 📊**: Furnish managers with an intuitive dashboard for viewing sales reports tailored to different time frames—daily, weekly, monthly, quarterly—or any custom date range, aiding in strategic planning.

8. **Inventory Filtering and Smart Search Capabilities 🔍**: Offer advanced inventory filtering options that allow sorting based on criteria like price, alphabetical listing, and others. The smart search function uses natural language processing to yield relevant results based on keyword input.

9. **Age-Restricted Sales Compliance Mechanism ⛔**: Integrate a robust system to automatically validate customer age for certain age-sensitive products like alcohol, thereby ensuring compliance with legal requirements and reducing potential liabilities. <br><br><br> <br><br><br>

## User Stories Revision 3.5

1. **Upcitemdb API Integration 🌐** ✅
   - User Story: As a manager, I need to use the upcitemdb API to fetch product information when adding a new product into the system.
   - Priority: High 10
   - Estimated Cost: 8

2. **Product Management 🛠️** ✅
   - User Story: As a manager, I want to manually input, delete, and update product information in our system, ensuring that duplicate barcodes or IDs are handled appropriately.
   - Priority: High 10
   - Estimated Cost: 8

3. **Barcode Integration 📸** ✅
   - User Story: As a store employee, I need to scan a product's barcode to quickly input or verify product information and handle scenarios where the barcode might not be in the system.
   - Priority: High 8
   - Estimated Cost: 5

4. **Checkout System with Enhancements 💰** ✅
   - User Story: As a cashier, I need the system to allow continuous product scanning, display cart details alphabetically, apply coupon codes, and update the inventory based on the transaction's success.
   - Priority: High 7
   - Estimated Cost: 5

5. **Sales Report 📊** ✅
   - User Story: As a manager, I want to receive daily/monthly/yearly sales reports to keep track of business performance.
   - Priority: High 6
   - Estimated Cost: 4

6. **Alert System 🔔** ✅
   - User Story: As a manager, I want the system to alert me via email about sales/price changes, low stock, and approaching product expiration dates.
   - Priority: Medium 5
   - Estimated Cost: 4

7. **Inventory Filtering and Search 🔍** ✅
   - User Story: As a store employee, I need to filter inventory based on various criteria and search for products by ID, name, or barcode to easily manage and locate items.
   - Priority: Medium 5
   - Estimated Cost: 3

8. **Sale Restrictions ⛔** ✅
   - User Story: As a manager, I want to set restrictions on sales of certain items based on location, customer age, etc., to comply with laws and regulations.
   - Priority: Medium 5
   - Estimated Cost: 3

9. **Newsletter 🗞️** ✅
   - User Story: As a customer, I want to subscribe to a newsletter for regular updates, promotions, and product recommendations.
   - Priority: Low 2
   - Estimated Cost: 1

Update: Customer Account System Integration 🌟

We've expanded our software to include essential customer account management now. This new feature allows for tracking purchase history and managing customer details directly within our platform, enhancing both employee and customer experiences.

## Additional Information 📌

This program requires a one-time setup of cURL. To install cURL on your system, you can use the following command in MSYS2 MINGW64:
```bash
pacman -S mingw-w64-x86_64-curl
```
For additional assistance or inquiries, please feel free to reach out to our technical support team at official.marketme@gmail.com. <br><br><br>








Burndown chart:
https://docs.google.com/spreadsheets/d/1PmEULxzyLe58C3dYHZTaqGl39ewRZSZj/edit#gid=2086879608

![image](https://github.com/CS179K-Summer23/cs179-project-marketme/assets/77028662/df374ed2-25ec-4f68-a2a6-dd62edf9160b)





