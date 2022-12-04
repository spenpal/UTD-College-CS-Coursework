SELECT * FROM Customer
 
 1.
 SELECT emp_first_name, emp_last_name, count(*)
 FROM Employee
 INNER JOIN Deliverer ON emp_id = area_man_id
 GROUP BY emp_id, emp_first_name, emp_last_name
 HAVING max(count(*))

2.

SELECT cust_first_name, cust_last_name
FROM OrdinaryCustomer INNER JOIN Order ON ord_mem_id = cust_id
GROUP BY cust_first_name, AVG(ord_subtotals);



