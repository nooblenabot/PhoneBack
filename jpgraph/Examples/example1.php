<?php
include ("../jpgraph.php");
include ("../jpgraph_line.php");

$ydata = array(11,3,8,12,5,1,9,13,5,7);
$ydata2 = array(12,3,6,12,5,8,9,15,5,7);
// Create the graph. These two calls are always required
$graph = new Graph(300,200,"auto");	
$graph->SetScale("textlin");
$graph->img->SetMargin(50,90,40,50);
$graph->xaxis->SetFont(FF_FONT1,FS_BOLD);
$graph->title->Set("Examples for graph");

// Create the linear plot
$lineplot=new LinePlot($ydata);
$lineplot->SetLegend("Test 1");
$lineplot->SetColor("blue");

$lineplot2=new LinePlot($ydata2);
$lineplot2->SetLegend("Test 2");
$lineplot2->SetColor("red");

// Add the plot to the graph
$graph->Add($lineplot);
$graph->Add($lineplot2);
// Display the graph
$graph->Stroke();
?>