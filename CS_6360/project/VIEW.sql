-- 1
-- Annual Top 3 Customers: This view returns the First Name, Last Name, Total Order Subtotals of the customers who paid top 3 total subtotals of all orders in past 1 year.
CREATE VIEW AnnualTopThree AS
Select Customer.cust_first_name, Customer.cust_last_name, SUM(Orders.ord_subtotals) AS sum_total
FROM Customer JOIN Orders ON Customer.cust_id = Orders.cust_id
WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 YEAR)
GROUP BY Orders.cust_id
ORDER BY sum_total DESC LIMIT 3;

-- 2
-- Popular Restaurant Type: This view returns the Type of restaurants that have the most number of orders in past 1 year. 
CREATE VIEW PopularRestaurantType AS
SELECT type, COUNT(type) AS num_orders
FROM Restaurant
JOIN Orders ON Restaurant.rest_id = Orders.shop_id
WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 YEAR)
GROUP BY type
ORDER BY num_orders DESC
LIMIT 1;

-- 3
--Potential Silver Member: This view returns the information of the customers (not a silver member yet) who have placed orders more than 10 times in the past 1 month. 
CREATE VIEW PotentialSilverMember AS
SELECT * 
FROM (
    Select Customer.cust_first_name, Customer.cust_last_name, Count(Orders.ord_subtotals) AS total_orders
    FROM Customer JOIN Orders ON Customer.cust_id = Orders.cust_id
    WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 MONTH) AND Customer.silver_mem_flag = false
    GROUP BY Orders.cust_id
    ORDER BY total_orders DESC
) AS count_total_per_customer
WHERE count_total_per_customer.total_orders > 10;

-- 4
-- Best Area Manager: This view returns the information of the area manager who successfully made the most number of contracts with shops in her/his working area in past 1 year.
CREATE VIEW BestAreaManager AS
SELECT Employee.emp_id, Employee.emp_first_name, Employee.emp_last_name, COUNT(Shop.shop_id) AS num_contracts
FROM Employee
JOIN AreaManager ON Employee.emp_id = AreaManager.area_man_id
JOIN Shop ON Employee.emp_id = Shop.area_man_id
WHERE Shop.contract_start_time > DATE_SUB(NOW(),INTERVAL 1 YEAR)
GROUP BY Employee.emp_id
ORDER BY num_contracts DESC
LIMIT 1;

-- 5
-- Top Restaurants: This view returns the top restaurant in each restaurant type that have the most orders in past 1 month.
CREATE VIEW TopRestaurants AS
SELECT type, shop_name, COUNT(shop_name) AS num_orders
FROM Restaurant
JOIN Orders ON Restaurant.rest_id = Orders.shop_id
JOIN Shop ON Restaurant.rest_id = Shop.shop_id
WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 MONTH)
GROUP BY type, shop_name
ORDER BY num_orders DESC;


-- For SQL Query #5
CREATE VIEW DelivererStats AS
SELECT emp_first_name, emp_last_name, orders_delivered
FROM Employee
JOIN (
    SELECT deliverer_id, COUNT(Delivers.ord_id) as orders_delivered
    FROM Delivers
    JOIN Orders ON Delivers.ord_id = Orders.ord_id
    WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 MONTH)
    GROUP BY deliverer_id
) AS deliverer_order_count 
ON Employee.emp_id = deliverer_order_count.deliverer_id
ORDER BY orders_delivered DESC;


-- For SQL Query #6
CREATE VIEW RestaurantPromo AS
SELECT shop_name, COUNT(shop_name) as total_promotions
FROM Shop
JOIN Restaurant ON Shop.shop_id = Restaurant.rest_id
JOIN Orders on Restaurant.rest_id = Orders.shop_id
JOIN Uses ON Orders.ord_id = Uses.ord_id
WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 MONTH)
GROUP BY Shop.shop_name
