/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package addressbook;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
//import java.util.Scanner;
/**
 *
 * @author Taehoo
 */
public class AddressBook {
   
    
    private ArrayList<Personal> personals;
    private int capacity;
    private int length;
    
    public AddressBook(){
        this.personals = new ArrayList<Personal>(256);
        this.capacity = 256;
        this.length =0;
    }
    
    public AddressBook(int capacity){
        this.personals = new ArrayList<Personal>(capacity);
        this.capacity = capacity;
        this.length = 0 ;
    }
    
    public int Record(String name, String address, String telephoneNumber, String emailAddress){
        Personal personal = new Personal(name, address, telephoneNumber, emailAddress);
        int index=this.length;
        
        if(this.length < this.capacity){
           this.personals.add(index, personal);
        }else{
            this.personals.add(personal);
            this.capacity++;
        }
        
        this.length++;
        return index;
        
    }
    public int[] Find(String name){
        int i=0;
        int j=0;
        int count=0;
        int [] indexes = new int[this.capacity];
        Personal personal;
        
        while(i<this.length) {//1. 주소록의 개수만큼 반복한다.
            personal = this.personals.get(i);//1.1 개인을 구한다.
            if(personal.GetName().compareTo(name) == 0){//1.2.이름을 비교한다.
                indexes[j] = i; //1.3. 같으면 배열에 저장하고 개수를센다.
                j++;
                count++;
            }
            i++;
        }
        indexes = Arrays.copyOf(indexes, count);//2. 배열을 복사한다.
        //3. 배열을 출력한다.
        return indexes;
    }
    
    public int Correct(int index, String address, String telephoneNumber, String emailAddress){
        Personal personal = new Personal(this.personals.get(index).GetName(), 
                           address, telephoneNumber, emailAddress);
        this.personals.set(index, personal);
        return index;
    }
    
    public int Erase(int index){
        this.personals.remove(index);
        this.length--;
        return -1;
    }
    
    public void Arrange(){
        AscendingPersonals ascendingPersonals = new AscendingPersonals();
        Collections.sort(this.personals, ascendingPersonals);
    }
    
    public Personal GetAt(int index){
        return this.personals.get(index);
    }
    
    public int GetCapacity(){
        return this.capacity;
    }
    public int GetLength(){
        return this.length;
    }
   
    /*public static void main(String[] args){
        Scanner sc=new Scanner(System.in);
        //1. 주소록을 생성하다.
        AddressBook addressBook=new AddressBook(2);
        //2. 홍길동, 서울시, 0101111, hong@을 기재하다.
        int index=addressBook.Record("하길동", "서울시", "0101111", "hong@");
        Personal personal=new Personal();
        personal=addressBook.GetAt(index);
        System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
        //3. 고길동, 대전시, 0102222, ko@을 기재하다.
        index=addressBook.Record("하길동", "대전시", "0102222", "ko@");
        personal=addressBook.GetAt(index);
        System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
        //4. 정길동, 대구시, 0103333, jung@을 기재하다.
        index=addressBook.Record("나길동", "대구시", "0103333", "jung@");
        personal=addressBook.GetAt(index);
        System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
        //5. 홍길동, 부산시, 0104444, gil@을 기재하다.
        index=addressBook.Record("가길동", "부산시", "0104444", "gil@");
        personal=addressBook.GetAt(index);
        System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
        //6. 홍길동을 찾다.
        int indexes[]= addressBook.Find("하길동");
        int i=0;
        while(i<indexes.length){
            personal=addressBook.GetAt(indexes[i]);
            System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
            i++;
        }
        
        //7. 세번쨰를 서울시로 바꾸다
        index = addressBook.Correct(2, "서울시", "01033333", "jung@");
        personal = addressBook.GetAt(index);
        System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
        //8. 두번째를 지우다.
        index = addressBook.Erase(1);
        if(index == -1){
            System.out.println("지워졌습니다. ");
        }
        //9. 정리하다.
        addressBook.Arrange();
        i=0;
        while(i< addressBook.GetLength()){
            personal=addressBook.GetAt(i);
            System.out.println(personal.GetName() +" "+ personal.GetAddress() +" "+ personal.GetTelephoneNumber() +" "+ personal.GetEmailAddress());
            i++;
        }
         
    }
*/
}

class AscendingPersonals implements Comparator<Personal> {
    @Override
    public int compare(Personal one, Personal other){
           return one.GetName().compareTo(other.GetName());
    }

}


