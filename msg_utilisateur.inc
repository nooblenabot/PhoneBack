
<!----------------------------------------------->
<!--       AFFICHAGE UTILISATEUR EN COURS      -->
<!----------------------------------------------->

<TR>
  <TD class="message">
        
    <?php
      // affichage de l'utilisateur de l'application
      $sql = "SELECT prenom FROM utilisateur WHERE id=".$_SESSION["id_utilisateur"] ;
      $res = $conn->Execute($sql) ;
      if (!$res)
      {
        echo "Erreur utilisateur : ".$conn->ErrorMsg() ;
      }
      else
      {
        echo "Utilisateur actuel : ".$res->fields[0] ;
      }
    ?>

  </TD>
</TR>

<!----------------------------------------------->

