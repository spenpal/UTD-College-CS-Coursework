-- 1
-- Find the names of employee who supervises the most number of deliverers. 
SELECT emp_first_name, emp_last_name, COUNT(deliverer_id) AS num_deliverers
FROM Employee
JOIN Deliverer ON Employee.emp_id = Deliverer.area_man_id
GROUP BY emp_first_name, emp_last_name
ORDER BY num_deliverers DESC
LIMIT 1;

-- 2
-- Find the average number of orders placed by Potential Silver Member (view).
SELECT AVG(total_orders) AS avg_orders
FROM PotentialSilverMember;

-- 3
-- Find all the customers who placed orders of the restaurants that belong to Popular Restaurant Type (view). Please also report the name of restaurants. 
SELECT DISTINCT Customer.cust_first_name, Customer.cust_last_name, Shop.shop_name
FROM Orders
JOIN Customer ON Orders.cust_id = Customer.cust_id
JOIN Shop ON Orders.shop_id = Shop.shop_id
JOIN Restaurant ON Shop.shop_id = Restaurant.rest_id
WHERE Restaurant.type = (
    SELECT type
    FROM PopularRestaurantType
    LIMIT 1
)
ORDER BY cust_first_name, cust_last_name;

-- 4
-- List all the customers that have become a silver member within a month of joining the system.
SELECT cust_first_name, cust_last_name
FROM Customer
JOIN SilverMember ON Customer.cust_id = SilverMember.silver_mem_id
WHERE Customer.silver_mem_flag = true AND TIMESTAMPDIFF(MONTH, join_date, silver_join_date) < 1;

-- 5
-- Find the names of deliverers who delivered the most orders in past 1 month.
SELECT *
FROM DelivererStats
WHERE orders_delivered = (
    SELECT MAX(orders_delivered)
    FROM DelivererStats
);

-- 6
-- Find the restaurants that provide most promotion in past 1 month.
SELECT *
FROM RestaurantPromo
WHERE total_promotions = (
    SELECT MAX(total_promotions)
    FROM RestaurantPromo
);

-- 7
-- Find the customer who have place orders of all restaurants that are fast food.
SELECT cust_first_name, cust_last_name
FROM (
    SELECT cust_first_name, cust_last_name, COUNT(DISTINCT shop_name) AS num_restaurants
    FROM Customer
    JOIN Orders ON Customer.cust_id = Orders.cust_id
    JOIN Shop ON Orders.shop_id = Shop.shop_id
    JOIN Restaurant ON Shop.shop_id = Restaurant.rest_id
    WHERE Restaurant.type = 'Fast Food'
    GROUP BY cust_first_name, cust_last_name
) as T
WHERE num_restaurants = (
    SELECT Count(*) as num_fast_food
    FROM Restaurant
    WHERE type = 'Fast Food'
);

-- 8
-- For each restaurant, list all the customers who placed the order there, and the price of each order.
SELECT Shop.shop_name, cust_first_name, cust_last_name, Orders.ord_subtotals
FROM Shop
JOIN Restaurant ON Shop.shop_id = Restaurant.rest_id 
JOIN Orders ON Restaurant.rest_id = Orders.shop_id
JOIN Customer ON Orders.cust_id = Customer.cust_id
ORDER BY Shop.shop_name, cust_first_name, cust_last_name;

-- 9
-- Find the area that have the most number of restaurants located.
SELECT rest_area, COUNT(rest_id) AS num_restaurants
FROM Restaurant
GROUP BY rest_area
ORDER BY num_restaurants DESC
LIMIT 1;

-- 10
-- Find the schedule of the restaurant that have the most orders in past 1 month. 
SELECT temp.shop_name, Schedule.shop_id, Schedule.day_of_week, Schedule.open_time, Schedule.close_time
FROM Schedule JOIN (
    SELECT Shop.shop_name, Shop.shop_id as id_of_shop, COUNT(Orders.ord_id) AS count_of_orders
    FROM Shop
    JOIN Restaurant ON Shop.shop_id = Restaurant.rest_id
    JOIN Orders ON Restaurant.rest_id = Orders.shop_id
    WHERE Orders.pay_time > DATE_SUB(NOW(),INTERVAL 1 MONTH)
    GROUP BY Shop.shop_id
    ORDER BY count_of_orders DESC
    LIMIT 1
) AS temp ON Schedule.shop_id = temp.id_of_shop;

-- 11
-- Find the names of employee who are also a Premium Member. 
SELECT emp_first_name, emp_last_name
FROM Employee
WHERE Employee.pre_mem_id IS NOT NULL;

-- 12
-- Find the supermarket that have most different products in stock. 
SELECT Shop.shop_name, COUNT(DISTINCT Product.prod_name) AS num_products
FROM Shop
JOIN Product ON Shop.shop_id = Product.super_id
GROUP BY shop_name
ORDER BY num_products DESC
LIMIT 1;

-- 13
-- For each product, list all the supermarket selling it, and the price of the product at the supermarket.
SELECT Product.prod_name, Shop.shop_name, Product.prod_price
FROM Product
JOIN Shop ON Product.super_id = Shop.shop_id
ORDER BY prod_name, Shop.shop_name;