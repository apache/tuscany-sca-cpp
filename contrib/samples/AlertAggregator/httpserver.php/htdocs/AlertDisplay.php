<!--
   Licensed to the Apache Software Foundation (ASF) under one
   or more contributor license agreements.  See the NOTICE file
   distributed with this work for additional information
   regarding copyright ownership.  The ASF licenses this file
   to you under the Apache License, Version 2.0 (the
   "License"); you may not use this file except in compliance
   with the License.  You may obtain a copy of the License at
   
     http://www.apache.org/licenses/LICENSE-2.0
     
   Unless required by applicable law or agreed to in writing,
   software distributed under the License is distributed on an
   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
   KIND, either express or implied.  See the License for the
   specific language governing permissions and limitations
   under the License.
-->
<?php
include 'SCA/SCA.php';

/**
 * @service
 * @binding.rest.rpc
 * @types http://tuscany.apache.org/samples/alerter ./Alerter.xsd
 */
class AlertDisplay
{
    /**
     * @reference
     * @binding.rest.rpc http://localhost:9090/rest/sample.alerter.AlerterComponent/AlerterService
     * @types http://tuscany.apache.org/samples/alerter ./Alerter.xsd
     */
	public $alert_service;
	
	/**
      * @return string
      */  
    public function getAlertsHTMLTable()
    {  
        $xmldas        = SDO_DAS_XML::create("./Alerter.xsd");
        $doc           = $xmldas->loadFile("./Cached_Alerts.xml");
        $cached_alerts = $doc->getRootDataObject();      
        
        // Use the alertService reference
        $new_alerts = $this->alert_service->getAllNewAlerts();
   
        foreach($new_alerts->alert as $alert){
            $new_alert = $cached_alerts->createDataObject('alert');
            $new_alert->title    = $alert->title;
            $new_alert->summary  = $alert->summary;
            $new_alert->address  = $alert->address;
            $new_alert->date     = $alert->date; 
            $new_alert->sourceid = $alert->sourceid;  
            $new_alert->unread   = true;         
        }
               
        $return_table = "<TABLE border=\'0\'>";  
        $alert_id_number = 0;
        foreach($cached_alerts->alert as $alert){
            $alert->id = "alert_" . $alert_id_number;  
            $title = substr($alert->title,0,80);
            $summary = substr($alert->summary,0,40);
            $unread_string = $alert->unread ? "unread_title" : "read_title";
            $return_row = <<<ALERTROW
<TR class="source_$alert->sourceid clickable" onclick="displayAlert('$alert->address', '')">
  <TD>
    <SPAN id="$alert->id" class="$unread_string">$title</SPAN>
    <SPAN class="summary"> - $summary ...</SPAN>
  </TD>
  <TD>
    $alert->date
  </TD>
</TR>
ALERTROW;
           $return_table     = $return_table . $return_row;
           $alert_id_number += 1;
        }   
        $return_table = $return_table . "</TABLE>";
        
        // as php is one shot we have to save away the alerts we want to cache
        $xmldas->saveFile($doc, "./Cached_Alerts.xml");
                
        return $return_table;
    }
    
    /**
      * @param string $alert_id
      * @return string
      */      
    public function readAlert($alert_id)
    {
        $xmldas        = SDO_DAS_XML::create("./Alerter.xsd");
        $doc           = $xmldas->loadFile("./Cached_Alerts.xml");
        $cached_alerts = $doc->getRootDataObject();
        
        $return_summary = "";

        foreach($cached_alerts->alert as $alert){
SCA::$logger->log($alert->id . " vs " . $alert_id );             
            if (strcmp($alert->id,$alert_id) == 0){
                SCA::$logger->log("Match");
                $alert->unread = false;
                $return_summary = "<PRE>" . $alert->summary . "</PRE>";
            }      
        }
        
        // as php is one shot we have to save away the alerts we want to cache
        $xmldas->saveFile($doc, "./Cached_Alerts.xml");
                
        return $return_summary;
    }

    /**
      * @return string
      */ 
    public function getAlertSourcesHTMLTable()
    {
        # Use the alertService reference
        $alert_sources = $this->alert_service->getAlertSources();

// some debug        
//ob_start();
//print_r( $alert_sources );
//$debug = ob_get_contents();
//ob_end_clean();
//SCA::$logger->log($debug);
        
        $return_table = "<TABLE border='0'>\n";        
    
        foreach ($alert_sources->source as $alert_source){
            $source_id = $alert_source->id;
            $return_row = <<<SOURCEROW
<TR CLASS="source_$source_id" >
  <TD CLASS="clickable" ONCLICK="displayAlert('$alert_source->address', '')">
    <IMG SRC="rss.png"/>&nbsp;&nbsp;$alert_source->name
  </TD>
  <TD CLASS="clickable link" ONCLICK="showEditSource('$source_id')">Edit</TD>
  <TD CLASS="clickable link" ONCLICK="deleteSource('$source_id')">Delete</TD>
</TR>
<TR ID="edit_source_$source_id" CLASS="hidden source_$source_id">
  <TD COLSPAN="3">
    <TABLE CLASS="sourceDetailsTable">
      <TR>
        <TD>Source name:</TD>
        <TD>
          <INPUT ID="source_{$source_id}_name" TYPE="TEXT" SIZE="50" VALUE="$alert_source->name"/>
        </TD>
      </TR>
      <TR>
        <TD>Source address:</TD>
        <TD>
          <INPUT ID="source_{$source_id}_address" TYPE="TEXT" SIZE="50" VALUE="$alert_source->address"/>
        </TD>
      </TR>
      <TR>
        <TD>
          <INPUT ID="source_{$source_id}_type" TYPE="HIDDEN" VALUE="$source_id"/>
          <INPUT TYPE="BUTTON" VALUE="Update" ONCLICK="updateSource('$source_id')"/>
          <INPUT TYPE="BUTTON" VALUE="Cancel" ONCLICK="hideEditSource('$source_id')"/>
        </TD>
      </TR>
    </TABLE>
  </TD>
</TR>
SOURCEROW;
            $return_table = $return_table . $return_row;        
        }

        $return_table = $return_table . "</TABLE>\n";
        return $return_table;
    }

    /**
      * @param integer $source_id
      * @return string
      */ 
    public function deleteAlertSource($source_id){
        # Use the alertService reference    
        $this->alertService->removeAlertSource($source_id);
        
        //TODO
    }
        
    /**
      * @param source $alert_source http://tuscany.apache.org/samples/alerter
      */ 
    public function addAlertSource($alert_source){
        # Use the alertService reference    
        $this->alertService->addAlertSource($alert_source);
    }

    /**
      * @param source $alert_source http://tuscany.apache.org/samples/alerter
      */ 
    public function updateAlertSource($alert_source){
        # Use the alertService reference    
        $this->alertService->updateAlertSource($alert_source);
    }
}
?>



