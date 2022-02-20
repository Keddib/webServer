<?php
	$env_vars = getenv();

	foreach($env_vars as $env)
	{
		echo $env . '<br>';
	}
?>
