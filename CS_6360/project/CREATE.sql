CREATE TABLE Customer (
    cust_id INT PRIMARY KEY,
    cust_first_name VARCHAR(50),
    cust_middle_name VARCHAR(50),
    cust_last_name VARCHAR(50),
    cust_phone_num VARCHAR(50),
    join_date DATE,
    ord_cust_flag BOOLEAN,
    silver_mem_flag BOOLEAN
);

CREATE TABLE CustomerAddress (
	cust_id INT,
	delivery_addr VARCHAR(50),
    PRIMARY KEY (cust_id, delivery_addr),
    FOREIGN KEY (cust_id) REFERENCES Customer(cust_id) ON DELETE CASCADE
);

CREATE TABLE OrdinaryCustomer (
	ord_mem_id INT PRIMARY KEY,
    FOREIGN KEY (ord_mem_id) REFERENCES Customer(cust_id) ON DELETE CASCADE
);

CREATE TABLE PremiumMember (
	pre_mem_id INT PRIMARY KEY,
	meal_pass_id VARCHAR(50),
	pass_start_date DATE,
	expiration_date DATE
);

CREATE TABLE SilverMember (
	silver_mem_id INT PRIMARY KEY,
	pre_mem_id INT,
	silver_join_date DATE,
    FOREIGN KEY (silver_mem_id) REFERENCES Customer(cust_id) ON DELETE CASCADE,
    FOREIGN KEY (pre_mem_id) REFERENCES PremiumMember(pre_mem_id) ON DELETE CASCADE
);

CREATE TABLE Employee (
	emp_id VARCHAR(50) PRIMARY KEY,
	emp_first_name VARCHAR(50),
	emp_middle_name VARCHAR(50),
	emp_last_name VARCHAR(50),
	emp_phone_num VARCHAR(50),
	start_date DATE,
	emp_addr VARCHAR(50),
    dob DATE,
	start_date_age INT DEFAULT(TIMESTAMPDIFF(YEAR, Employee.dob, start_date)),
	gender VARCHAR(50),
	pre_mem_id INT,
    FOREIGN KEY (pre_mem_id) REFERENCES PremiumMember(pre_mem_id) ON DELETE CASCADE,
	CHECK (REGEXP_LIKE(emp_id, '^E[0-9]{3}$')),
	CHECK (start_date_age >= 18)
);

CREATE TABLE EmployeePhone (
	emp_id VARCHAR(50),
	emp_phone_num VARCHAR(50),
	PRIMARY KEY (emp_id, emp_phone_num),
	FOREIGN KEY (emp_id) REFERENCES Employee(emp_id) ON DELETE CASCADE
);

CREATE TABLE Staff (
	staff_id VARCHAR(50) PRIMARY KEY,
	FOREIGN KEY (staff_id) REFERENCES Employee(emp_id) ON DELETE CASCADE
);

CREATE TABLE AreaManager (
	area_man_id VARCHAR(50) PRIMARY KEY,
	work_area VARCHAR(50),
	FOREIGN KEY (area_man_id) REFERENCES Employee(emp_id) ON DELETE CASCADE
);

CREATE TABLE Deliverer (
	deliverer_id VARCHAR(50) PRIMARY KEY,
	area_man_id VARCHAR(50),
	FOREIGN KEY (deliverer_id) REFERENCES Employee(emp_id) ON DELETE CASCADE,
	FOREIGN KEY (area_man_id) REFERENCES AreaManager(area_man_id) ON DELETE CASCADE
);

CREATE TABLE Vehicle (
	plate_num VARCHAR(50) PRIMARY KEY,
	maker VARCHAR(50),
	model VARCHAR(50),
	color VARCHAR(50),
	deliverer_id VARCHAR(50),
	FOREIGN KEY (deliverer_id) REFERENCES Deliverer(deliverer_id) ON DELETE CASCADE
);

CREATE TABLE MemberCard (
	card_id INT PRIMARY KEY,
	issue_date DATE,
	silver_mem_id INT,
	issued_by VARCHAR(50),
	FOREIGN KEY (silver_mem_id) REFERENCES SilverMember(silver_mem_id) ON DELETE CASCADE,
	FOREIGN KEY (issued_by) REFERENCES Staff(staff_id) ON DELETE CASCADE
);

CREATE TABLE Shop (
	shop_id INT PRIMARY KEY,
	shop_name VARCHAR(50),
	shop_addr VARCHAR(50),
	bus_phone_num VARCHAR(50),
	con_start_time DATE,
	area_man_id VARCHAR(50),
	FOREIGN KEY (area_man_id) REFERENCES AreaManager(area_man_id) ON DELETE CASCADE
);

CREATE TABLE Orders (
	ord_id INT PRIMARY KEY,
	ord_contents VARCHAR(50),
	ord_subtotals DECIMAL(10,2),
	pay_confirm_num VARCHAR(50),
	pay_type VARCHAR(50),
	pay_time DATE,
	cust_id INT,
	shop_id INT,
	FOREIGN KEY (cust_id) REFERENCES Customer(cust_id) ON DELETE CASCADE,
	FOREIGN KEY (shop_id) REFERENCES Shop(shop_id) ON DELETE CASCADE
);

-- Change pay_confirm_num to a varchar type
ALTER TABLE Orders
MODIFY pay_confirm_num VARCHAR(50);


CREATE TABLE Promotion (
	promo_code VARCHAR(50) PRIMARY KEY,
	promo_desc VARCHAR(50),
	shop_id INT,
	FOREIGN KEY (shop_id) REFERENCES Shop(shop_id) ON DELETE CASCADE
);

CREATE TABLE Comment (
	com_id INT PRIMARY KEY,
	rating DECIMAL(3,2),
	comment_contents TEXT,
	cust_id INT,
	shop_id INT,
	FOREIGN KEY (cust_id) REFERENCES Customer(cust_id) ON DELETE CASCADE,
	FOREIGN KEY (shop_id) REFERENCES Shop(shop_id) ON DELETE CASCADE
);

CREATE TABLE Restaurant (
	rest_id INT PRIMARY KEY,
	rest_area VARCHAR(50),
	type VARCHAR(50),
	FOREIGN KEY (rest_id) REFERENCES Shop(shop_id) ON DELETE CASCADE
);

CREATE TABLE Supermarket (
	super_id INT PRIMARY KEY,
	FOREIGN KEY (super_id) REFERENCES Shop(shop_id) ON DELETE CASCADE
);

CREATE TABLE Schedule (
	shop_id INT,
	day_of_week VARCHAR(50),
	open_time VARCHAR(50),
	close_time VARCHAR(50),
	PRIMARY KEY (shop_id, day_of_week),
	FOREIGN KEY (shop_id) REFERENCES Shop(shop_id) ON DELETE CASCADE
);

CREATE TABLE Product (
	super_id INT,
	prod_id INT,
	prod_name VARCHAR(50),
	prod_desc TEXT,
	prod_price DECIMAL(10,2),
	prod_count INT,
	PRIMARY KEY (super_id, prod_id),
	FOREIGN KEY (super_id) REFERENCES Supermarket(super_id) ON DELETE CASCADE
);

CREATE TABLE Uses (
	cust_id INT,
	ord_id INT,
	promo_code VARCHAR(50),
	PRIMARY KEY (cust_id, ord_id, promo_code),
	FOREIGN KEY (cust_id) REFERENCES Customer(cust_id) ON DELETE CASCADE,
	FOREIGN KEY (ord_id) REFERENCES Orders(ord_id) ON DELETE CASCADE,
	FOREIGN KEY (promo_code) REFERENCES Promotion(promo_code) ON DELETE CASCADE
);

CREATE TABLE Delivers (
	ord_id INT PRIMARY KEY,
	deliverer_id VARCHAR(50),
	plate_num VARCHAR(50),
	FOREIGN KEY (ord_id) REFERENCES Orders(ord_id) ON DELETE CASCADE,
	FOREIGN KEY (deliverer_id) REFERENCES Deliverer(deliverer_id) ON DELETE CASCADE,
	FOREIGN KEY (plate_num) REFERENCES Vehicle(plate_num) ON DELETE CASCADE
);