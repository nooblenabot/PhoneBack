<?php 
//===========================================================================
// GESTION DES IMAGES ET DU LOGO
//===========================================================================

//------------------------------------------------------------------
//    Fonction qui affiche le formulaire de gestion des images
//    de l'entête et du bas de page.

function affiche_formulaire_images()
{
  $barre_bouton = Barre_de_boutons_BO("Charger", "Effacer", "Retour");
  $puce= "<strong>&#155;&nbsp;</strong>";
  if (file_exists("images/custom/entete-fond.gif")) 
  {
  	$url_supprimer_img_entete = "<span class=\"custo\">".$puce."Vous avez une image d'entête personnalisée.";
  	$url_supprimer_img_entete .= "<a href=\"index.php?action=administrer.supprime_image&zone=ENT&choix=7\"><img src=\"images/delete.gif\" title=\"supprimer l'image\" /></a></span>";
  }
  
  if (file_exists("images/custom/baspage-fond.gif")) 
  {
  	$url_supprimer_img_bas = "<span class=\"custo\">".$puce."Vous avez une image de bas de page personnalisée.";
  	$url_supprimer_img_bas .= "<a href=\"index.php?action=administrer.supprime_image&zone=BAS&choix=7\"><img src=\"images/delete.gif\" title=\"supprimer l'image\" /></a></span>";
  }
  
  if (file_exists("images/custom/baspage-logo.gif")) 
  {
  	$url_supprimer_img_logo = "<span class=\"custo\">".$puce."Vous avez un logo personnalisé.";
  	$url_supprimer_img_logo .= "<a href=\"index.php?action=administrer.supprime_image&zone=LOG&choix=7\"><img src=\"images/delete.gif\" title=\"supprimer l'image\" /></a></span>";
  }
  
  $html = <<<END
  
  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      return true ;
    }
  </SCRIPT>


    <FORM method="POST" name="AjoutImage" onSubmit="return verifie_formulaire()" ENCTYPE="multipart/form-data">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.uploade_image">
     <INPUT TYPE="hidden" NAME="choix" VALUE="7">
      <H2>Gestion du logo et des images d'entête et de bas de page :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>Choisissez votre image d'entête<br/>
          <ul><li>Largeur : 700 pixels</li><li>hauteur : 150 pixels</li></ul>$url_supprimer_img_entete<br/>
          </TD>
          <TD><INPUT type="file" name="p_img_entete" value=""></TD>
        </TR>
        <TR>
          <TD>Choisissez votre image de bas de page
          <ul><li>Largeur : 700 pixels</li><li>hauteur : de 50 pixels mini à 150 pixels maxi</li></ul>$url_supprimer_img_bas<br/>
          </TD>
          <TD><INPUT type="file" name="p_img_baspage" value=""></TD>
        </TR>
        <TR>
          <TD>Choisissez le logo de votre société<br/>
          <ul><li>Largeur : de 50 pixels mini à 400 pixels</li><li>hauteur : de 50 pixels</li></ul>$url_supprimer_img_logo<br/>
          </TD>
          <TD><INPUT type="file" name="p_img_logo" value=""></TD>
        </TR>
      </TABLE>
      <BR>
      <CENTER>
        $barre_bouton
      </CENTER>
    </FORM>
END;

return(array($html, null)) ;
}


//------------------------------------------------------------------
//    Fonction qui vérifie l'extension d'un fichier

function verif_extension($origin, $ext_a_verifier)
{
	$origin = strtolower(basename($origin));
	$fileext = (strpos($origin,'.')===false?'':'.'.substr(strrchr($origin, "."), 1));
	if (strtoupper($fileext) == ".".strtoupper($ext_a_verifier))  return true;
	else return false;
}

//------------------------------------------------------------------
//    Fonction qui place l'image an bon endroit

function move_image ($tmp_name,$name,$definitive_name,$commentOK, $commentNOK, $commentNoGif)
{
	if (is_uploaded_file($tmp_name)) 
	{
		// vérifier que l'extensionde l'image est bien GIF
		if (verif_extension($name,'GIF')) 
		{
			if (move_uploaded_file($tmp_name, $definitive_name)) {
			     $msg  = "<br/>".$commentOK.".";
			} 
			else {
			   $msg  = "<br/>".$commentNOK.".";
			}
		}
		else
		{
			$msg  = "<br/>".$commentNoGif.".";
		}
	}
	return $msg;	
}

//------------------------------------------------------------------
//    Fonction qui upload les images entete, bas de page et logo.

function uploade_image ()
{
	global $redirect ;
	global $action ;
	global $footer_msg ;
  
	$uploaddir = 'images/custom/';
	$img_entete = $uploaddir . 'entete-fond.gif';
	$img_baspage = $uploaddir . 'baspage-fond.gif';
	$img_logo = $uploaddir . 'baspage-logo.gif';
	
	// Gestion de l'upload de l'image d'entête.
	$footer_msg  = move_image ($_FILES['p_img_entete']['tmp_name'],$_FILES['p_img_entete']['name'],$img_entete,"Image d'entête chargée", "Problème au chargement de l'image d'entête", "L'image d'entête doit être un GIF");
	
	// Gestion de l'upload de l'image de bas de page.
	$footer_msg  .= move_image ($_FILES['p_img_baspage']['tmp_name'],$_FILES['p_img_baspage']['name'],$img_baspage,"Image de bas de page chargée", "Problème au chargement de l'image de bas de page", "L'image de bas de page doit être un GIF");
	
	// Gestion de l'upload de l'image de logo.
	$footer_msg  .= move_image ($_FILES['p_img_logo']['tmp_name'],$_FILES['p_img_logo']['name'],$img_logo,"Logo chargé", "Problème au chargement du logo", "Le logo doit être un GIF");

  $redirect = true;
  $action = "administrer.choix";
}

//------------------------------------------------------------------
//    Fonction qui supprime une image

function supprime_image ($zone)
{
  global $redirect ;
  global $action ;
  global $footer_msg ;
  if ($zone == "ENT") $nom_fichier = "entete-fond.gif";
  if ($zone == "BAS") $nom_fichier = "baspage-fond.gif";
  if ($zone == "LOG") $nom_fichier = "baspage-logo.gif";
  if (file_exists("images/custom/".$nom_fichier))
  {
	  if (unlink ("images/custom/".$nom_fichier)) $footer_msg = "Fichier image supprimé.";
	  else $footer_msg = "Problème lors de la suppression du fichier image.";
  }
  
  $redirect = true;
  $action = "administrer.choix";
}


?>