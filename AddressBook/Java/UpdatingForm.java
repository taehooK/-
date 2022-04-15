/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package addressbook;

import java.sql.*;
import javax.swing.table.DefaultTableModel;
/**
 *
 * @author Taehoo
 */
public class UpdatingForm extends javax.swing.JFrame {
    AddressBook addressBook;
    
    public UpdatingForm()  {
        initComponents();
        this.addressBook = new AddressBook(10000);
        Personal personal;
        String number;
        Load();
        // 로드한게 있으면
       if(this.addressBook.GetLength() > 0){
           int i=0;
           DefaultTableModel model = (DefaultTableModel)Table_Personals.getModel();
           while(i<this.addressBook.GetLength()){
               personal= this.addressBook.GetAt(i);
               number = Integer.toString(i+1);
               model.addRow(new Object[]{
                   number, personal.GetName(), personal.GetAddress(), personal.GetTelephoneNumber(),
                   personal.GetEmailAddress()
               });
               
               i++;
           }
       }
   }
    public void Load() {
        Connection conn = null;
        Statement stmt = null;
        ResultSet rs = null;
        String name;
        String address;
        String telephoneNumber;
        String emailAddress;
        
        String sql = "SELECT Personal.name, Personal.address, Personal.TelephoneNumber, Personal.emailAddress"
                + " FROM Personal;";
        try{
            conn = DriverManager.getConnection("jdbc:mysql://localhost/AddressBook?useSSL=false&severTimezone=UTC",
                "root", "kim653");
            stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);
            
            while(rs.next()){
                name = rs.getString(1);
                address = rs.getString(2);
                telephoneNumber = rs.getString(3);
                emailAddress = rs.getString(4);
                
                this.addressBook.Record(name, address, telephoneNumber, emailAddress);
            }
            rs.close();
            stmt.close();
            conn.close();        
            
        }catch(SQLException ex){
            ex.printStackTrace();
        }
        
        
    }
    
    public void Save() {
        Connection conn = null;
        Statement stmt = null;
        ResultSet rs = null;
        int i=0;
        Personal personal;
        String sql;
        String code;
        
        try{
         conn = DriverManager.getConnection("jdbc:mysql://localhost/AddressBook?useSSL=false&severTimezone=UTC", 
                 "root", "kim653");
         stmt = conn.createStatement();
         sql = "SELECT Personal.code FROM Personal";
         rs = stmt.executeQuery(sql);
         stmt = conn.createStatement();
         sql = "DELETE FROM Personal";
         stmt.executeUpdate(sql);
         
         while(i<this.addressBook.GetLength() && rs.next()){
             code = rs.getString(1);
             personal = this.addressBook.GetAt(i);
             sql = String.format("INSERT INTO Personal(code, name, address,"
                     +" telephoneNumber, emailAddress) "+
                     "VALUES('%s', '%s', '%s', '%s', '%s');", code, personal.GetName(),
                     personal.GetAddress(), personal.GetTelephoneNumber(), personal.GetEmailAddress());
             stmt.executeUpdate(sql);
    
             i++;
         }
         rs.close();
         stmt.close();
         conn.close();
         
        }catch(SQLException ex){
            ex.printStackTrace();
        }
        
        
    }
    
    public void Insert(int index) {
        String code= MakeCode();
        Personal personal = this.addressBook.GetAt(index);
        Connection conn= null;
        Statement statement=null;
        
     
        String sql;
        sql=String.format("INSERT INTO Personal(code, name, address, telephoneNumber, emailAddress)" +
                " VALUES('%s', '%s', '%s', '%s', '%s');", code, personal.GetName(), personal.GetAddress(),
                personal.GetTelephoneNumber(), personal.GetEmailAddress());
        
        try{
            conn = DriverManager.getConnection("jdbc:mysql://localhost/AddressBook?useSSL=false&serverTimezone=UTC",
                "root", "kim653");
            statement = conn.createStatement();
            statement.executeUpdate(sql);
            
            statement.close();
            conn.close();
        }catch(SQLException se){
            se.printStackTrace();
        }
        
           
    }
    public void Delete(int index){
        Connection conn = null;
        Statement stmt = null;
        String sql;
        ResultSet rs=null;
        String code;
        
        try{
            conn = DriverManager.getConnection("jdbc:mysql://localhost/AddressBook?useSSL=false&serverTimezone=UTC",
                "root", "kim653");
            stmt = conn.createStatement();
            sql = "SELECT Personal.code FROM Personal;";
            rs = stmt.executeQuery(sql);
            
            rs.first();
            rs.relative(index);
            
            code=rs.getString(1);
            sql = String.format("DELETE FROM Personal WHERE code='%s'", code);
            stmt.executeUpdate(sql);
            
            rs.close();
            stmt.close();
            conn.close();
        }catch(SQLException ex){
            ex.printStackTrace();
        }
        
        
    }
    
    public void Modify(int index){
        Connection conn = null;
        Statement statement =null;
        ResultSet rs= null;
        String code;
        String sql;
        Personal personal = this.addressBook.GetAt(index);
        
        try{
            conn = DriverManager.getConnection("jdbc:mysql://localhost/AddressBook?useSSL=false&serverTimezone=UTC",
                "root", "kim653");
            statement = conn.createStatement();
            sql = "SELECT Personal.code FROM Personal;";
            
            rs = statement.executeQuery(sql);
            rs.first();
            rs.relative(index);
            code =rs.getString(1);
            sql = String.format("UPDATE Personal SET address='%s',"+
                    "telephoneNumber='%s', emailAddress='%s' "+
                    "WHERE Personal.code='%s';", personal.GetAddress(), personal.GetTelephoneNumber(),
                    personal.GetEmailAddress(), code);
            statement.executeUpdate(sql);
            
            rs.close();
            statement.close();
            conn.close();
            
        }catch(SQLException ex){
            ex.printStackTrace();
        }
        
        
    }
    public String MakeCode(){
        Connection conn = null;
        Statement stmt= null;
        ResultSet rs = null;
        String sql;
        String code ="P0001";
        
        try{
            conn = DriverManager.getConnection("jdbc:mysql://localhost/AddressBook?useSSL=false&serverTimezone=UTC",
                "root", "kim653"); // 커넥션
            stmt = conn.createStatement(); // 정보 저장
            sql = "SELECT Personal.code FROM Personal ORDER BY code DESC;";
            rs = stmt.executeQuery(sql);
            
            if(rs.next()){
                code = rs.getString(1);
                code = code.substring(1);
                int codeNumber = Integer.parseInt(code) +1;
                code = String.format("P%04d", codeNumber);
            }
            rs.close();
            stmt.close();
            conn.close();
        }catch(SQLException ex){
            ex.printStackTrace();
        }finally{
           //finally block used to close resources
           try{
              if(stmt!=null)
                 stmt.close();
           }catch(SQLException se2){
           }// nothing we can do
           try{
              if(conn!=null)
                 conn.close();
           }catch(SQLException se){
              se.printStackTrace();
           }//end finally try
        }//end try
        
   
        return code;
    }
    

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jInternalFrame1 = new javax.swing.JInternalFrame();
        jDialog1 = new javax.swing.JDialog();
        jDialog2 = new javax.swing.JDialog();
        jDialog3 = new javax.swing.JDialog();
        jDialog4 = new javax.swing.JDialog();
        jDialog5 = new javax.swing.JDialog();
        jDialog6 = new javax.swing.JDialog();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTable1 = new javax.swing.JTable();
        Text_Name = new javax.swing.JTextField();
        Text_Address = new javax.swing.JTextField();
        Text_TelephoneNumber = new javax.swing.JTextField();
        Text_EmailAddress = new javax.swing.JTextField();
        Label_Name = new javax.swing.JLabel();
        Label_address = new javax.swing.JLabel();
        Label_TelephoneNumber = new javax.swing.JLabel();
        Label_EmailAddress = new javax.swing.JLabel();
        RecordButton = new javax.swing.JButton();
        FindButton = new javax.swing.JButton();
        CorrectButton = new javax.swing.JButton();
        EraseButton = new javax.swing.JButton();
        ArrangeButton = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        Table_Personals = new javax.swing.JTable();

        jInternalFrame1.setVisible(true);

        javax.swing.GroupLayout jInternalFrame1Layout = new javax.swing.GroupLayout(jInternalFrame1.getContentPane());
        jInternalFrame1.getContentPane().setLayout(jInternalFrame1Layout);
        jInternalFrame1Layout.setHorizontalGroup(
            jInternalFrame1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        jInternalFrame1Layout.setVerticalGroup(
            jInternalFrame1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jDialog1Layout = new javax.swing.GroupLayout(jDialog1.getContentPane());
        jDialog1.getContentPane().setLayout(jDialog1Layout);
        jDialog1Layout.setHorizontalGroup(
            jDialog1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        jDialog1Layout.setVerticalGroup(
            jDialog1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jDialog2Layout = new javax.swing.GroupLayout(jDialog2.getContentPane());
        jDialog2.getContentPane().setLayout(jDialog2Layout);
        jDialog2Layout.setHorizontalGroup(
            jDialog2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        jDialog2Layout.setVerticalGroup(
            jDialog2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jDialog3Layout = new javax.swing.GroupLayout(jDialog3.getContentPane());
        jDialog3.getContentPane().setLayout(jDialog3Layout);
        jDialog3Layout.setHorizontalGroup(
            jDialog3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        jDialog3Layout.setVerticalGroup(
            jDialog3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jDialog4Layout = new javax.swing.GroupLayout(jDialog4.getContentPane());
        jDialog4.getContentPane().setLayout(jDialog4Layout);
        jDialog4Layout.setHorizontalGroup(
            jDialog4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        jDialog4Layout.setVerticalGroup(
            jDialog4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jDialog5Layout = new javax.swing.GroupLayout(jDialog5.getContentPane());
        jDialog5.getContentPane().setLayout(jDialog5Layout);
        jDialog5Layout.setHorizontalGroup(
            jDialog5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        jDialog5Layout.setVerticalGroup(
            jDialog5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jDialog6Layout = new javax.swing.GroupLayout(jDialog6.getContentPane());
        jDialog6.getContentPane().setLayout(jDialog6Layout);
        jDialog6Layout.setHorizontalGroup(
            jDialog6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        jDialog6Layout.setVerticalGroup(
            jDialog6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        jTable1.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null}
            },
            new String [] {
                "Title 1", "Title 2", "Title 3", "Title 4"
            }
        ));
        jScrollPane2.setViewportView(jTable1);

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("주소록");
        setName("주소록"); // NOI18N
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                formWindowClosing(evt);
            }
        });

        Text_Name.setToolTipText("");

        Text_Address.setCursor(new java.awt.Cursor(java.awt.Cursor.TEXT_CURSOR));

        Text_EmailAddress.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Text_EmailAddressActionPerformed(evt);
            }
        });

        Label_Name.setText("성명");

        Label_address.setText("주소");

        Label_TelephoneNumber.setText("전화번호");

        Label_EmailAddress.setText("이메일주소");

        RecordButton.setText("기재하기");
        RecordButton.setActionCommand("RECORDBUTTON");
        RecordButton.setFocusable(false);
        RecordButton.setIconTextGap(0);
        RecordButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                RecordButtonActionPerformed(evt);
            }
        });

        FindButton.setText("찾기");
        FindButton.setActionCommand("FINDBUTTON");
        FindButton.setFocusable(false);
        FindButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                FindButtonActionPerformed(evt);
            }
        });

        CorrectButton.setText("고치기");
        CorrectButton.setActionCommand("CORRECTBUTTON");
        CorrectButton.setFocusable(false);
        CorrectButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CorrectButtonActionPerformed(evt);
            }
        });

        EraseButton.setText("지우기");
        EraseButton.setActionCommand("ERASEBUTTON");
        EraseButton.setFocusable(false);
        EraseButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                EraseButtonActionPerformed(evt);
            }
        });

        ArrangeButton.setText("정리하기");
        ArrangeButton.setActionCommand("ARRANGEBUTTON");
        ArrangeButton.setFocusable(false);
        ArrangeButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ArrangeButtonActionPerformed(evt);
            }
        });

        Table_Personals.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "번호", "성명", "주소", "전화번호", "이메일주소"
            }
        ));
        Table_Personals.setCellSelectionEnabled(false);
        Table_Personals.setRowSelectionAllowed(true);
        Table_Personals.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        Table_Personals.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                Table_PersonalsMouseClicked(evt);
            }
        });
        jScrollPane1.setViewportView(Table_Personals);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(24, 24, 24)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(Label_Name)
                                    .addComponent(Label_address))
                                .addGap(117, 117, 117)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(Text_Address, javax.swing.GroupLayout.PREFERRED_SIZE, 200, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addGap(0, 0, Short.MAX_VALUE))
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(Text_Name, javax.swing.GroupLayout.DEFAULT_SIZE, 199, Short.MAX_VALUE)
                                        .addGap(145, 145, 145))))
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(Label_TelephoneNumber)
                                    .addComponent(Label_EmailAddress))
                                .addGap(78, 78, 78)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                    .addComponent(Text_TelephoneNumber)
                                    .addComponent(Text_EmailAddress, javax.swing.GroupLayout.DEFAULT_SIZE, 200, Short.MAX_VALUE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 429, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(ArrangeButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(RecordButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(FindButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(CorrectButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(EraseButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(29, 29, 29))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(28, 28, 28)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(RecordButton)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(Label_Name)
                        .addComponent(Text_Name, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(18, 18, 18)
                        .addComponent(FindButton))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(27, 27, 27)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(Text_Address, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Label_address))))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(34, 34, 34)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(37, 37, 37)
                                .addComponent(EraseButton)
                                .addGap(27, 27, 27)
                                .addComponent(ArrangeButton))
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(Text_TelephoneNumber, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(Label_TelephoneNumber))
                                .addGap(18, 18, 18)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(Label_EmailAddress)
                                    .addComponent(Text_EmailAddress, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(45, 45, 45)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 195, javax.swing.GroupLayout.PREFERRED_SIZE))))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(18, 18, 18)
                        .addComponent(CorrectButton)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        getAccessibleContext().setAccessibleDescription("");

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void RecordButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_RecordButtonActionPerformed
       String name = Text_Name.getText();
       String address = Text_Address.getText();
       String telephoneNumber = Text_TelephoneNumber.getText();
       String emailAddress = Text_EmailAddress.getText();
       int index;
       String number;
       Personal personal;
      
       index = this.addressBook.Record(name, address, telephoneNumber, emailAddress);
       Insert(index);
       
       
       number = Integer.toString(index+1);
       personal = this.addressBook.GetAt(index);
       
       DefaultTableModel model = (DefaultTableModel)Table_Personals.getModel();
       model.addRow(new Object [] {number, name, address, telephoneNumber, emailAddress});
       Table_Personals.changeSelection(index, index, false, false);
  
    }//GEN-LAST:event_RecordButtonActionPerformed

    private void Text_EmailAddressActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_Text_EmailAddressActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_Text_EmailAddressActionPerformed

    private void FindButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_FindButtonActionPerformed
        FindingForm findingForm = new FindingForm(this);
        findingForm.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        findingForm.setVisible(true);
    }//GEN-LAST:event_FindButtonActionPerformed

    private void CorrectButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CorrectButtonActionPerformed

        String address;
        String telephoneNumber;
        String emailAddress;
        Personal personal;
        //4.1. 개인을 읽는다.
 
        address = Text_Address.getText();
        telephoneNumber = Text_TelephoneNumber.getText();
        emailAddress = Text_EmailAddress.getText();
        
        //4.2. 리스트뷰 컨트롤의 선택된 항목의 위치를 읽는다.
        int index = Table_Personals.getSelectedRow();
        
        //4.3. 주소록에서 고친다.
        index = this.addressBook.Correct(index, address, telephoneNumber, emailAddress);
        Modify(index);
        personal = this.addressBook.GetAt(index);
        //4.4. 리스트뷰 컨트롤의 항목을 수정한다.
        Table_Personals.setValueAt(personal.GetAddress(), index, 2);
        Table_Personals.setValueAt(personal.GetTelephoneNumber(), index, 3);
        Table_Personals.setValueAt(personal.GetEmailAddress(), index, 4);
        
    }//GEN-LAST:event_CorrectButtonActionPerformed

    private void EraseButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_EraseButtonActionPerformed
        String number;
        DefaultTableModel model = (DefaultTableModel)Table_Personals.getModel();
        int index = Table_Personals.getSelectedRow();
        this.addressBook.Erase(index);
        Delete(index);
        model.removeRow(index);
   
        while(index<this.addressBook.GetLength()){
            number = Integer.toString(index+1);
            Table_Personals.setValueAt(number, index, 0);
            index++;
        }
    }//GEN-LAST:event_EraseButtonActionPerformed

    private void ArrangeButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ArrangeButtonActionPerformed
       int i;
       Personal personal;
       DefaultTableModel model = (DefaultTableModel)Table_Personals.getModel();
       String number;
       
       this.addressBook.Arrange();
       Save();
       model.setNumRows(0);
       
       i=0;
       while(i<this.addressBook.GetLength()){
           personal = this.addressBook.GetAt(i);
           number = Integer.toString(i+1);
           model.addRow(new Object[] {number, personal.GetName(),
               personal.GetAddress(), personal.GetTelephoneNumber(), personal.GetEmailAddress()
           });
           i++;
       }
       
       
    }//GEN-LAST:event_ArrangeButtonActionPerformed

    private void Table_PersonalsMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_Table_PersonalsMouseClicked
        int index = Table_Personals.getSelectedRow();
        String name;
        String address;
        String telephoneNumber;
        String emailAddress;
        
        name = (String)Table_Personals.getValueAt(index, 1);
        address = (String)Table_Personals.getValueAt(index, 2);
        telephoneNumber = (String)Table_Personals.getValueAt(index, 3);
        emailAddress = (String)Table_Personals.getValueAt(index, 4);
        
        Text_Name.setText(name);
        Text_Address.setText(address);
        Text_TelephoneNumber.setText(telephoneNumber);
        Text_EmailAddress.setText(emailAddress);
    }//GEN-LAST:event_Table_PersonalsMouseClicked

    private void formWindowClosing(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosing
        Save();
    }//GEN-LAST:event_formWindowClosing

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(UpdatingForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(UpdatingForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(UpdatingForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(UpdatingForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new UpdatingForm().setVisible(true);
            }
        });
    }
    
 
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton ArrangeButton;
    private javax.swing.JButton CorrectButton;
    private javax.swing.JButton EraseButton;
    private javax.swing.JButton FindButton;
    private javax.swing.JLabel Label_EmailAddress;
    private javax.swing.JLabel Label_Name;
    private javax.swing.JLabel Label_TelephoneNumber;
    private javax.swing.JLabel Label_address;
    private javax.swing.JButton RecordButton;
    javax.swing.JTable Table_Personals;
    javax.swing.JTextField Text_Address;
    javax.swing.JTextField Text_EmailAddress;
    public javax.swing.JTextField Text_Name;
    javax.swing.JTextField Text_TelephoneNumber;
    private javax.swing.JDialog jDialog1;
    private javax.swing.JDialog jDialog2;
    private javax.swing.JDialog jDialog3;
    private javax.swing.JDialog jDialog4;
    private javax.swing.JDialog jDialog5;
    private javax.swing.JDialog jDialog6;
    private javax.swing.JInternalFrame jInternalFrame1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTable jTable1;
    // End of variables declaration//GEN-END:variables
}
