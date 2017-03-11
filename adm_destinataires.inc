<?php
//====================================================
// Gestion des destinataires d'appel
//
// Ce qui est intéressant, c'est de pouvoir charger 
// tous les gens du service d'un seul coup, sur
// la base d'un fichier CSV.
//====================================================

//----------------------------------------------------
//    Fonction qui affiche la liste des destinataires
//----------------------------------------------------
function affiche_liste_destinataires()
{
	  global $conn ;
	  $barre_bouton = Barre_de_boutons_BO("","","Retour");  
	  $sql = "SELECT id, nom, prenom, tel, mail, defaut FROM destinataire ORDER BY 2" ;
	  $res = $conn->Execute($sql) ;
	  if($conn->ErrorNo() != 0)
	  {
	    echo $conn->ErrorMsg();
	  }
	  else
	  {
	    echo "<CENTER>" ;
	    echo "<H2>Liste des destinataires :</H2>" ;
	    echo "<A HREF=\"index.php?action=administrer.affiche_formulaire_destinataire\">Ajouter</A>" ;
	    echo "<BR><BR>" ;
	    echo "<TABLE class=\"RESULTAT\"><TR><TH>Nom</TH><TH>Prénom</TH><TH>Téléphone</TH><TH>Email</TH><TH>par d&eacute;faut<br/>dans la liste</TH><TH colspan=\"2\">Actions</TH></TR>" ;
	    while (!$res->EOF)
	    {
		  if ($res->fields[5] == "oui" ) { $class_defval = " class='default_value'"; }
	      else {  $class_defval ="";  }
	      echo "<TR><TD>".$res->fields[1]."</TD><TD>".$res->fields[2]."</TD><TD>".$res->fields[3]."</TD><TD><small>".$res->fields[4]."</small></TD>";
	      echo "<TD align=center".$class_defval.">".$res->fields[5]."</TD><TD>&nbsp;</TD>";
	      echo "<TD align=center><A HREF=index.php?action=administrer.affiche_formulaire_destinataire&iddest=".$res->fields[0]."&choix=8><IMG SRC=\"images/edit.gif\" border=0></A>&nbsp;&nbsp;&nbsp;" ;
	      echo "<A HREF=index.php?action=administrer.supprime_destinataire&iddest=".$res->fields[0]."&choix=8><IMG SRC=\"images/delete.gif\" border=0></A></TD></TR>" ;
	      $res->MoveNext() ;
	    }
	    echo "</TABLE><br><br>" ;
		echo $barre_bouton;
	    echo "</CENTER>" ;
	  }
}
	
//----------------------------------------------------
// Fonction qui affiche le formulaire d'upload 
// du fichier CSV
//----------------------------------------------------
function affiche_formulaire_upload ()
{
  $barre_bouton = Barre_de_boutons_BO("Charger", "Effacer", "");
  $puce= "<strong>&#155;&nbsp;</strong>";
  
  $html = <<<END
  
  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      return true ;
    }
  </SCRIPT>

    <FORM method="POST" name="ChargerDest" onSubmit="return verifie_formulaire()" ENCTYPE="multipart/form-data">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.upload_destinataires">
     <INPUT TYPE="hidden" NAME="choix" VALUE="8">
      <H2>Chargement de l'annuaire de votre service <br/>pour remplir la liste de destinataires des appels :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>Fichier au format CSV&nbsp;&nbsp;&nbsp;&nbsp;
          <INPUT type="file" name="p_annuaire" value="" size="40"></TD>
        </TR>
        <TR>
          <TD>
           Voici un exemple de fichier au format reconnu par le système. Le caractère de séparation est le point-virgule.
           L'odre des données est important : <span class="default_value">nom</span class="default_value">, <span class="default_value">prénom</span>, <span class="default_value">mail</span>, <span class="default_value">téléphone</span>.
           <div style="margin:10px;padding:5px;border:1px solid black;background-color:black;color:green;">
           <code>
           Durand;Robert;rdurand@mondomaine.com;Poste 327<br/>
           Dufour;Josiane;jdufour@mondomaine.com;7997<br/>
           Duchmol;Jean Jacques;jduch@mondomaine.com;35-55<br/>
           </code>
           </div><br/></TD>
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

//----------------------------------------------------
// Fonction qui affiche le formulaire d'ajout ou 
// modification d'un destinataire
//----------------------------------------------------
function affiche_formulaire_destinataire ($p_iddest="")
{
  global $conn ;
  $etoile = chpobli();

  if ($p_iddest == "" )   {
	  $lib_action = "Insérer";
  }
  else  {
	  $lib_action = "Modifier";
	  // Init des valeurs du formulaire
	  $sql = "SELECT id, nom, prenom, mail, tel, defaut FROM destinataire WHERE id = ".$p_iddest ;
	  $res = $conn->Execute($sql) ;
	  if($conn->ErrorNo() != 0)
	  {
	    return ($conn->ErrorMsg()) ;
	  }
	  $p_iddest = $res->fields[0];
	  $p_nom = $res->fields[1];
	  $p_prenom = $res->fields[2];
	  $p_mail = $res->fields[3];
	  $p_tel = $res->fields[4];
	  $p_defaut = $res->fields[5];
  }
  $barre_bouton = Barre_de_boutons_BO($lib_action, "Effacer", "Retour");  
  $menu_oui_non = ListeDeChoix("SELECT distinct 'oui', 'oui' FROM type union SELECT distinct 'non', 'non' FROM type order by 1","p_defaut", $p_defaut) ;

    $html = <<<END
  
      <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      if (document.AjoutDest.p_nom.value == "" ) {alert ('Le champ "nom" est vide !'); document.AjoutDest.p_nom.focus(); return false;}
      if (document.AjoutDest.p_mail.value == "" ) {alert ('Le champ "Email" est vide !'); document.AjoutDest.p_mail.focus(); return false;}
      // vérification que l'arobase se trouve dans le champ email,
      //              que l'arobase n'est pas en première ou en dernière position de la chaine de caractères
      var mailutil = window.document.AjoutDest.p_mail.value;
      var place = mailutil.indexOf("@");
      if ( (place == -1) || (place == 0) || (place == ((mailutil.length)-1)) )
      {
        alert ("Veuillez entrer une adresse e-mail valide !") ;
        document.AjoutDest.p_mail.focus();
        return false ;
      }
      return true ;
    }
  </SCRIPT>

    <FORM method="POST" name="AjoutDest" onSubmit="return verifie_formulaire()">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.inserer_modifier_destinataire">
     <INPUT TYPE="hidden" NAME="choix" VALUE="8">
     <INPUT TYPE="hidden" NAME="iddest" VALUE="$p_iddest">
      <H2>Veuillez saisir ou modifier la structure appelante :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>Nom$etoile</TD>
          <TD><INPUT type="text" name="p_nom" value="$p_nom"></TD>
        </TR>
        <TR>
          <TD>Prénom</TD>
          <TD><INPUT type="text" name="p_prenom" value="$p_prenom"></TD>
        </TR>
        <TR>
          <TD>Email$etoile</TD>
          <TD><INPUT type="text" name="p_mail" value="$p_mail"></TD>
        </TR>
        <TR>
          <TD>Téléphone</TD>
          <TD><INPUT type="text" name="p_tel" value="$p_tel"></TD>
        </TR>
        <TR>
          <TD>sélectionner ce type d'appel<br/> par défaut dans le formulaire<br/> de prise des appels</TD>
          <TD>$menu_oui_non</TD>
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


//----------------------------------------------------
//  Fonction qui insère ou modifie un destinataire 
//----------------------------------------------------
function inserer_modifier_destinataire($p_id = "")
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  // si p_defaut = Oui, alors on met tous les autres enregistrements à "Non"
  if ($_REQUEST["p_defaut"] == 'oui' ) 
  {
  	$sql = "UPDATE destinataire set defaut = 'non'";
  	$res = $conn->Execute($sql) ;
  }
  if ($p_id == "")
  {
	  $sql = "INSERT INTO destinataire ( nom, prenom, mail, tel, defaut ) ".
          "VALUES (".$conn->qstr($_REQUEST["p_nom"]).",".$conn->qstr($_REQUEST["p_prenom"]).",".$conn->qstr($_REQUEST["p_mail"]).",".$conn->qstr($_REQUEST["p_tel"]).",".$conn->qstr($_REQUEST["p_defaut"]).")" ;
      $lib_action = "l'insertion";
      $participe_passe = "ins&eacute;r&eacute;e";
  }
  else 
  {
	  $sql = "UPDATE destinataire set nom = '".$_REQUEST["p_nom"]."', prenom = '".$_REQUEST["p_prenom"]."', mail = '".$_REQUEST["p_mail"]."', tel = '".$_REQUEST["p_tel"]."' , defaut = '".$_REQUEST["p_defaut"]."' WHERE id = ".$p_id;
      $lib_action = "la modification";
      $participe_passe = "modifi&eacute;";
  }
  $res = $conn->Execute($sql) ;

  if ($res == true)
  {
    $footer_msg = "Un destinataire a bien &eacute;t&eacute; ".$participe_passe." dans la base" ;
  }
  else
  {
     $footer_msg = "Il y a eu un probl&egrave;me lors de ".$lib_action." dans la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}



//----------------------------------------------------
// Fonction qui supprime un destinataire
//----------------------------------------------------
function supprime_destinataire ($pid_dest)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $choix ;
  global $footer_msg ;
  $sql0 = "DELETE from destinataire WHERE id = ".$pid_dest;
  $res = $conn->Execute($sql0) ;
  if ($res == true)
  {
    $footer_msg = "Le destinataire a bien &eacute;t&eacute; supprim&eacute; dans la base" ;
  }
  else
  {
     $footer_msg = "Il y a eu un probl&egrave;me lors de la suppression dans la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}

//----------------------------------------------------
// Fonction qui charge le fichier CSV
//----------------------------------------------------
function upload_destinataires () 
{
   global $conn;
   global $redirect ;
   global $action ;
   global $footer_msg ;
   $separateur = ";";
   $uploaddir = 'tmp/';
   
   $redirect = true ;
   $action = "administrer.choix" ;  
  
   	if (is_uploaded_file($_FILES['p_annuaire']['tmp_name'])) 
	{
   		$fichier_source = $uploaddir.$_FILES['p_annuaire']['name'];
		if (move_uploaded_file($_FILES['p_annuaire']['tmp_name'], $fichier_source))
		     $footer_msg  = "<br/>Fichier \"destinataires\" chargé.";
		else $footer_msg  = "<br/>Problème de chargement de votre fichier \"destinataires\".";
		
		// charger le fichier dans un tableau
		$fc=file($fichier_source);
		
		// Contrôle du fichier   : Nb de colonnes
		foreach($fc as $line)
		{
			$mytab = explode ( ";", $line);
			if (count($mytab) <> 4 )
			{
				$footer_msg = "Des lignes de votre fichier ne comportent pas le bon nombre de champs ( ".count($mytab)." au lieu de 4).";
				return;
			}
        }

		// Ouvrir le fichier en écriture
		$f=fopen($fichier_source,"w");
		
		// traitement du fichier : Ajout de l'id en début de ligne et de la colonne "defaut" à la fin.
		foreach($fc as $line)
		{
		    fputs($f,";".$line); //placer $line de nouveau dans le fichier
		}
		fclose($f);
	 
	   // suppression de toutes les données de la table
	   $sql0 = "delete from destinataire";
	   $res0 = $conn->Execute($sql0);
	   // Chargement des nouveaux destinataires.
	   $sql1 = "LOAD DATA INFILE 'D:/travail/www-local/Appels/".$fichier_source."' INTO TABLE destinataire FIELDS TERMINATED BY '".$separateur."' ENCLOSED BY '\"' ESCAPED BY '\\\\' LINES TERMINATED BY '\\n'";
	   $res1 = $conn->Execute($sql1);
		  if($conn->ErrorNo() != 0)
		  {
		   $footer_msg.= $conn->ErrorMsg();
		  }

	   // suppression du fichier uploadé
	   unlink($fichier_source);
   	}
}

?>