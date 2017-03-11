<?php

//*************************************************************************************************
// SOUS-CONTROLEUR DE L'ADMINISTRATION      
// 
// Ce sous-contrôleur gère toutes les fonctionnalités back-office          
// C'est-à-dire toutes les fonctions décrites dans les fichiers adm_xxxxx.inc.                      
//*************************************************************************************************

//==================================================================
// VARIABLES GLOBALES
//==================================================================

$choix = $_REQUEST["choix"];

//==================================================================
// INCLUSIONS GLOBALES
//==================================================================

include "adm_base.inc";
include "adm_utilisateurs.inc";
include "adm_droits.inc";
include "adm_destinations.inc";
include "adm_structures.inc";
include "adm_typesappels.inc";
include "adm_issuesappels.inc";
include "adm_images.inc";
include "adm_destinataires.inc";


//==================================================================
// FONCTIONS GENERIQUES
//==================================================================
function Barre_de_boutons_BO ($lib_valider="",$lib_effacer="",$lib_retour="") 
{
  if ($lib_valider != "" ) { $bouton_valider = Bouton($lib_valider); }
  if ($lib_effacer != "" ) { $bouton_reset = Bouton($lib_effacer, "Reset"); }
  if ($lib_retour != "" ) { $bouton_retour = Bouton_Retour("index.php?action=administrer"); }
  return $bouton_retour."&nbsp;&nbsp;&nbsp;".$bouton_reset."&nbsp;&nbsp;&nbsp;".$bouton_valider;
}


//==================================================================
// FONCTIONS DU BACK-OFFICE.
//==================================================================
//------------------------------------------------------------------
//    Fonction qui affiche un formulaire de choix entre :
//        - la suppression dans la base
//        - l'ajout d'un nouvel utilisateur
//        - l'ajout d'un destinataire par défaut en fonction du type d'appel

function choix_menu_administrer()
{
  $separateur = "        <TR><TD colspan='2'><hr/></TD></TR>";

  $html = <<<END
    <script>
    function go (oRad) {
	    FrmChoix.submit();
    }
   </script>
    
    <FORM method="GET" name="FrmChoix">
    <H1>Je d&eacute;sire :</H1>
    <BR>
      <TABLE align="center">
        <TR>
          <TD colspan="2"><INPUT TYPE="hidden" NAME="action" VALUE="administrer.choix"></TD>
        </TR>
        <TR>
          <TD><INPUT type="radio" name="choix" value="1" onClick="go(this);"></TD>
          <TD>Nettoyer la base des appels (tout ou partiellement)</TD>
        </TR>
		$separateur
        <TR>
          <TD><INPUT type="radio" name="choix" value="2" onClick="go(this);"></TD>
          <TD>G&eacute;rer les utilisateurs de l'application</TD>
        </TR>
        <TR>
          <TD><INPUT type="radio" name="choix" value="6" onClick="go(this);"></TD>
          <TD>G&eacute;rer les droits d'accès</TD>
        </TR>
        <TR>
          <TD><INPUT type="radio" name="choix" value="8" onClick="go(this);"></TD>
          <TD>G&eacute;rer les destinataires des appels</TD>
        </TR>
		$separateur
        <TR>
          <TD><INPUT type="radio" name="choix" value="5" onClick="go(this);"></TD>
          <TD>G&eacute;rer les types d'appels</TD>
        </TR>
        <TR>
          <TD><INPUT type="radio" name="choix" value="3" onClick="go(this);"></TD>
          <TD>G&eacute;rer les destinations d'appel par défaut</TD>
        </TR>
        <TR>
          <TD><INPUT type="radio" name="choix" value="9" onClick="go(this);"></TD>
          <TD>G&eacute;rer les issues des appels</TD>
        </TR>
        <TR>
          <TD><INPUT type="radio" name="choix" value="4" onClick="go(this);"></TD>
          <TD>G&eacute;rer les structures appelantes</TD>
        </TR>
		$separateur
        <TR>
          <TD><INPUT type="radio" name="choix" value="7" onClick="go(this);"></TD>
          <TD>G&eacute;rer le logo et les images d'entête et de bas de page</TD>
        </TR>
	    $separateur
      </TABLE>
    </FORM>
END;

  return(array($html, null)) ;
}


//===========================================================================
// PROGRAMME PRINCIPAL
//===========================================================================

function administrer_switch($action)
{
  global $_REQUEST ;
  global $choix;
  
  switch ($action)
  {
    //=========================================================
    //=========================================================
    case "administrer":
      // on affiche le formulaire de choix d'administration
      $res = choix_menu_administrer();
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]) ;
      }
      break;
    //---------------------------------------------------------
    case "administrer.choix":
      if ($choix==1)
      {
        // on affiche formulaire de destruction de données
        $res = administration_destruction_formulaire ($_REQUEST["p_date"]) ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==2)
      {
        // on affiche le formulaire d'ajout d'utilisateur
        $res = affiche_liste_utilisateurs() ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==5)
      {
        // on affiche le formulaire de gestion des types d'appel
        $res = affiche_liste_typesappels() ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==4)
      {
        // on affiche le formulaire d'ajout de structures appelantes
        $res = affiche_liste_structures() ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==6)
      {
        // on affiche le formulaire de gestion des droits d'accès
        $res = affiche_matrice_droits() ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==7)
      {
        // on affiche le formulaire de gestion des images d'entête et de bas de page
        $res = affiche_formulaire_images() ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==8)
      {
        // on affiche le formulaire de gestion des destinataires
        affiche_liste_destinataires();
        $res = affiche_formulaire_upload() ;
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      elseif ($choix==9)
      {
        // on affiche le formulaire de gestion des destinataires
        $res = affiche_liste_issuesappels();
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      else  // choix 3
      {
        // on affiche le formulaire de gestion des destinations d'appel
        $res = affiche_liste_destinations();
        if($res[1] != null)
        {
          print error($res[1]) ;
        }
        else
        {
          print ($res[0]) ;
        }
      }
      break;
    //=========================================================
    //=========================================================
    case "administrer.detruire":
      // on détruit les données de la base
      $res = detruire ($_REQUEST["p_date"]) ;
      $action = "administrer.verifdestruction";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.verifdestruction":
      // on vérifie la bonne destruction des données de la base
      if($res != null)
      {
        print error($res);
      }
      break;
    //=========================================================
    //=========================================================
    case "administrer.afficheformulaire":
      // on affiche le formulaire d'ajout d'utilisateur
      $res=administration_ajoututilisateur_formulaire() ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.insertutilisateur":
      // on insert le nouvel utilisateur
      $res = insertutilisateur($_REQUEST["p_nom"],$_REQUEST["p_prenom"],$_REQUEST["p_login"],$_REQUEST["p_motpasse1"],$_REQUEST["p_mail"]) ;
      $action = "administrer.verifinsertutilisateur";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.verifinsertutilisateur":
      // on vérifie la bonne insertion d'utilisateur
      if($res != null)
      {
        print error($res);
      }
      break;
    //---------------------------------------------------------
    case "administrer.modifieutilisateur":
      global $conn ;
      $sql = "SELECT nom, prenom, login, motpasse, mail, id FROM utilisateur WHERE id=".$_REQUEST["idutilisateur"] ;
      $rs=$conn->Execute($sql) ;
      $res = administration_ajoututilisateur_formulaire($rs->fields[0],$rs->fields[1], $rs->fields[2],$rs->fields[3], $rs->fields[3], $rs->fields[4],$rs->fields[5]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.applimodifsutilisateur":
      $res = modifie_utilisateur($_REQUEST["p_nom"],$_REQUEST["p_prenom"],$_REQUEST["p_login"],$_REQUEST["p_motpasse1"],$_REQUEST["p_mail"],$_REQUEST["idutilisateur"]) ;
      $redirect=true ;
      break ;
    //---------------------------------------------------------
    case "administrer.supprimeutilisateur":
      // on supprime l'utilisateur de la base
      $res = supprime_utilisateur();
      $action = "administrer.verifsuppresionutilis";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.verifsuppresionutilis":
      // on véifie la bonne suppression de l'utilisateur
      if($res != null)
      {
        print error($res);
      }
      break;
    //=========================================================
    //=========================================================
    case "administrer.affiche_formulaire_destination":
      $res=administration_ajoutdestination_formulaire() ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.insere_destination":
      // on insert la nouvelle destination d'appel par défaut.
      $res = insere_destination($_REQUEST["idtype"],$_REQUEST["iddestinataire"]) ;
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.supprime_destination":
      $res = supprime_destination();
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    //=========================================================
    case "administrer.affiche_formulaire_structure":
      $res=affiche_formulaire_structure() ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.inserer_structure":
      // on insert la nouvelle structure.
      $res = inserer_structure($_REQUEST["p_nom_struct"],$_REQUEST["p_defaut"]) ;
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.supprime_structure":
      $res = supprime_structure();
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    //=========================================================
    case "administrer.affiche_formulaire_typeappel":
      $res=affiche_formulaire_typeappel($_REQUEST["idtype"]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.inserer_modifier_typeappel":
      // on insert la nouvelle structure.
      $res = inserer_modifier_typeappel($_REQUEST["idtype"]) ;
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.supprime_typeappel":
      $res = supprime_typeappel();
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    //=========================================================
    case "administrer.modifie_droit":
      $res=modifie_droit($_REQUEST["iddroit"],$_REQUEST["anciendroit"]) ;
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    //=========================================================
    case "administrer.uploade_image":
      $res=uploade_image() ;
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    case "administrer.supprime_image":
      $res=supprime_image($_REQUEST["zone"]) ;
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    //=========================================================
    case "administrer.affiche_formulaire_upload":
      $res=affiche_formulaire_upload();
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    case "administrer.affiche_formulaire_destinataire":
      $res=affiche_formulaire_destinataire($_REQUEST["iddest"]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.upload_destinataires":
      $res=upload_destinataires();
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    case "administrer.supprime_destinataire":
      $res=supprime_destinataire($_REQUEST["iddest"]);
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    case "administrer.inserer_modifier_destinataire":
      $res=inserer_modifier_destinataire($_REQUEST["iddest"]);
      $action = "administrer.choix";
      $redirect = true;
      break ;
    //---------------------------------------------------------
    //=========================================================
    case "administrer.affiche_formulaire_issueappel":
      $res=affiche_formulaire_issueappel($_REQUEST["idissue"]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //---------------------------------------------------------
    case "administrer.insere_modifie_issueappel":
      // on insert la nouvelle structure.
      $res = insere_modifie_issueappel($_REQUEST["idissue"]) ;
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    case "administrer.supprime_issueappel":
      $res = supprime_issueappel();
      $action = "administrer.choix";
      $redirect = true;
      break;
    //---------------------------------------------------------
    default:
    $action="administrer";
  }
}

?>


