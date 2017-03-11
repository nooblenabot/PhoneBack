<?php

//*************************************************************************************************
// FICHIER DE GENERATION DE PDF                                                                   *
// S'APPUYANT SUR LES FICHIERS :                                                                  *
// fig1a.php, fig1b.php, fig2a.php, fig3a.php, fig3b.php                                          *
//*************************************************************************************************
// utilisation des variables de session
session_cache_limiter('private');

session_start();



// inclusion de la classe fpdf
include("pdf/fpdf.php");

// inclusion du fichier de configuration
// besoin de : SERVER_NAME, SCRIPT_NAME, FPDF_FONTPATH
include "config.inc";


function gen_pdf()
{

  // Création du PDF
  $pdf=new FPDF();
  $pdf->Open();
  $pdf->AddPage();
  // titre
  $pdf->SetFont('Arial','B',16);
  //                                  premier 1 : texte encadré
  //                                    deuxième 1 : retour à la ligne
  $pdf->Cell(190,10, 'STATISTIQUES :',1,1,'C');


  // on gère chaque cas du panier
  foreach($_SESSION["panier"] as $graph)
  {
    // saut de ligne
    $pdf->ln();
    // titre de la statistique
    $pdf->Cell(190,10, $graph[3],0,1,'C');
    // période de la statistique
    $pdf->Cell(190,10, "Du ".$graph[1]." au ".$graph[2],0,1,'C');

    switch($graph[0])
    {
      case "graphe1":
        // graphique fig1a
        $pdf->Image("http://".SERVER_NAME."/".SCRIPT_NAME."/fig1a.php?_jpg_csimd=1".$graph[4], 30,60,150, null, 'PNG') ;
        // graphique fig1b
        $pdf->Image("http://".SERVER_NAME."/".SCRIPT_NAME."/fig1b.php?_jpg_csimd=1".$graph[5], 30,120,150, null, 'PNG') ;
        // on ajoute une page
        $pdf->AddPage();
        break;
      case "graphe2":
        // graphique fig2a
        $pdf->Image("http://".SERVER_NAME."/".SCRIPT_NAME."/fig2a.php?_jpg_csimd=1".$graph[4], 30,60,150, null, 'PNG') ;
        // on ajoute une page
        $pdf->AddPage();
        break;
      case "graphe3":
        // graphique fig3a
        $pdf->Image("http://".SERVER_NAME."/".SCRIPT_NAME."/fig3a.php?_jpg_csimd=1".$graph[4], 30,60,150, null, 'PNG') ;
        // graphique fig3b
        $pdf->Image("http://".SERVER_NAME."/".SCRIPT_NAME."/fig3b.php?_jpg_csimd=1".$graph[5], 30,150,150, null, 'PNG') ;
        // on ajoute une page
        $pdf->AddPage();
        break;
        //graphique fig4
      case "graphe4":
	//Ci dessous, la version qui pose un problème       
        //$pdf->Image("http://".SERVER_NAME."/".SCRIPT_NAME."/fig4.php?_jpg_csimd=1&MYSID=".urlencode(session_id()) , 30,60,150, null, 'PNG');
        //Ci dessous, on met un message d'indisponibilté temporaire pour ce graphique
        $pdf->ln(30);
        $pdf->Cell(0,0, 'Graphique momentanément indisponible',0,1,'C');
        // on ajoute une page
        $pdf->AddPage();
        break;
    }
  }
  

  
  // on génère le PDF
  $pdf->Output();
  // suppression du fichier
  unlink($file);
}





// appel de la fonction de génération de PDF
gen_pdf();

?>
