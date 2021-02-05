select INPUT_DESCRIPTION, MEASUREMENT_TIME, VALUE
from inputs i 
join 
(
	SELECT *
	FROM counters_ull_time_data
	
	UNION
	
	SELECT *
	FROM counters_ll_time_data
) all_data
WHERE 
i.id_input = all_data.id_input
ORDER BY MEASUREMENT_TIME
