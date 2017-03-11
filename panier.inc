<?php

//*************************************************************************************************
// SOUS-CONTROLEUR DE PANIER                                                                      *
// + fonction d'ajout au panier                                                                   *
// + fonction de listage du panier                                                                *
// + fonction de vidage complet du panier                                                         *
// + fonction de suppression d'une statistique du panier                                          *
//*************************************************************************************************


//============================================================
// FONCTIONS
//============================================================


//------------------------------------------------------------
// Fonction d'ajout au panier

function ajout_panier($type, $date_debut, $date_fin, $titre, $grapha_data, $graphb_data=null)
{
  if(!isset($_SESSION["panier"]))
  {
    $_SESSION["panier"] = array() ;
  }
  array_push($_SESSION["panier"], array($type, $date_debut, $date_fin, $titre, $grapha_data, $graphb_data)) ;
  // redirection vers le contenu du panier
  $url="index.php?action=panier.lister" ;
  header("Location: $url") ;
}


//------------------------------------------------------------
// Fonction de listage du panier

function lister_panier()
{
  global $conn ;

  echo "<CENTER>" ;
  // dans le cas où le panier serait vide
  if ($_SESSION["panier"]==NULL)
  {
    echo "<H2>Votre panier est vide</H2>" ;
  }
  // sinon on affiche le contenu du panier
  else
  {
    echo "<TABLE class=\"RESULTAT\"><TR><TH>Graphique</TH><TH>Date 1</TH><TH>Date 2</TH><TH>Titre</TH><TH>&nbsp;</TH></TR>\n" ;
    $i=0 ;
    if(isset($_SESSION["panier"]))
    {
     // on génère dynamiquement le contenu du panier
     foreach($_SESSION["panier"] as $graph)
     {
       echo "<TR><TD>".$graph[0]."</TD><TD>".$graph[1]."</TD><TD>".$graph[2]."</TD><TD>".$graph[3]."</TD><TD><a href=\"index.php?action=panier.supprimerligne&ligne=".$i."\">Supprimer</a></TD></TR>\n" ;
       $i=$i+1 ;
     }
    }
    echo "</TABLE>\n" ;
    echo "<CENTER/>" ;
    echo "<BR><BR>" ;
    echo "<IMG SRC=\"images/pdf.png\">&nbsp;<A HREF=\"pdf.php\" TARGET=\"PDF\">G&eacute;n&eacute;rer PDF</A>&nbsp;&nbsp;|&nbsp;" ;
    echo "<A HREF=\"?action=panier.vider\">Vider panier</A>&nbsp;&nbsp;|&nbsp;" ;
  }
  // pour revnir au bon  type de graphe depuis le listage du panier
  // on récupère le chiffre en bout de chaine du type de graphique présent dans les statistiques
  $numgraph = substr( $graph[0],strlen($graph[0])-1,strlen($graph[0])-1 ) ;
  if ($_SESSION["panier"] != NULL)
  {
    // retour au graphique qui a été dernièrement ajouté en statistique
    echo "<A HREF=\"?action=states.choix&choix=".$numgraph."&date1=".$_SESSION["date1"]."&date2=".$_SESSION["date2"]."\">Retour</A>" ;
  }
  else
  {
    // retour à la première page de statistiques
    echo "<A HREF=\"?action=states&date1=".$_SESSION["date1"]."&date2=".$_SESSION["date2"]."\">Retour</A>" ;
  }
  echo "<BR><BR>" ;
}


//------------------------------------------------------------
// Fonction de vidage complet du panier

function vider_panier()
{
  unset($_SESSION["panier"]) ;
  // redirection vers le contenu du panier
  $url="index.php?action=panier.lister" ;
  header("Location: $url") ;
}


//------------------------------------------------------------
// Fonction de suppression d'une statistique du panier

function supprimerligne_panier($ligne)
{
  for ($i=$ligne ; $i<count($_SESSION["panier"]) ; $i=$i+1)
  {
    if (isset($_SESSION["panier"][$i])) $_SESSION["panier"][$i]=$_SESSION["panier"][$i+1] ;
  }
  unset($_SESSION["panier"][$i-1]) ;
  // redirection vers le contenu du panier
  $url="index.php?action=panier.lister" ;
  header("Location: $url") ;
}



//============================================================
// PROGRAMME PRINCIPAL
//============================================================


function panier_switch($action)
{
  global $conn ;
  global $_REQUEST ;
  global $action ;
  global $redirect ;
  global $footer_msg ;


  switch ($action)
  {
    //-------------------------------------------------
    case "panier.ajouter":
      // on ajoute une statistique au panier
      $res = ajout_panier($_REQUEST["type"],$_SESSION["date1"], $_SESSION["date2"], $_REQUEST["titre"], $_SESSION["graphea"], $_SESSION["grapheb"]);
      break;
    //-------------------------------------------------
    case "panier.verifajout":
      // on vérifie l'ajout au panier
      if($res != null)
      {
        print error($res);
      }
      $redirect = true;
      break;
    //-------------------------------------------------
    case "panier.lister":
      // on affiche le contenu du panier
      lister_panier();
      break;
    //-------------------------------------------------
    case "panier.vider":
      // on vide le panier
      vider_panier();
      break;
    //-------------------------------------------------
    case "panier.supprimerligne":
      // on supprime une statistique du panier
      supprimerligne_panier($_REQUEST["ligne"]);
      break;
    //-------------------------------------------------
    default:
      $action = "states";
  }
}

?>
