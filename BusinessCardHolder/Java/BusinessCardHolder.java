/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package businesscardholder;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.ListIterator;

/**
 *
 * @author Taehoo
 */
public class BusinessCardHolder {
    public BusinessCardHolder(){
        this.businessCards = new LinkedList<BusinessCard>();
        this.length = 0;
        this.current = null;
    }
    
    public BusinessCard PutIn(BusinessCard businessCard){
       this.businessCards.addLast(businessCard);
       this.length++;
       BusinessCard index = this.businessCards.getLast();
       this.current = index;
       
       return this.current;
    }
    
    public BusinessCard[] Find(String name){
        BusinessCard[] indexes = new BusinessCard[this.length];
        ListIterator<BusinessCard> listIterator = this.businessCards.listIterator();
        BusinessCard current;
        int count=0;
        
        
        while(listIterator.hasNext()){
            current = listIterator.next();
            if(current.GetPersonalName().compareTo(name) == 0){
                indexes[count] = current;
                count++;
            }
        }
        // 연결리스트에 처음부터 끝까지 반복하면서
        // 매개변수로받은 이름과 명함에 사람이름하고 같으면
        // 인덱시즈에 저장함
        indexes = Arrays.copyOf(indexes, count);
        
        return indexes;
 
    }
    public BusinessCard FindByCompanyName(String companyName){
        // 연결리스트에서 처음부터 끝까지 그리고 명함의 상호가 매개변수 상호랑 다를동안 반복함
        int index =0;
        BusinessCard businessCard = null;
        
        this.current = this.businessCards.getFirst();
        
        while(index<this.businessCards.size() && this.current.GetCompanyName().compareTo(companyName) !=0){
            this.current = this.businessCards.get(index);
            index++;
        }
        if(index<this.businessCards.size()){
            businessCard = this.current;
        }
        return businessCard;
    }
    public BusinessCard PutOut(BusinessCard index){
        int i = this.businessCards.indexOf(index);
        BusinessCard businessCard;
        businessCard = this.businessCards.get(i);
        this.businessCards.remove(i);
        this.length++;
        if(this.businessCards.size() !=0){
            this.current = this.businessCards.get(i);
        }else{
            this.current = null;
        }
        return businessCard;
    }
    public BusinessCard First(){
        this.current = this.businessCards.getFirst();
        
        return this.current;
    }
    public BusinessCard Previous(){
        int index = this.businessCards.indexOf(this.current);
        index--;
        if(index < 0 ){
            index = 0;
        }
        
        this.current = this.businessCards.get(index);
        return this.current;
    }
    public BusinessCard Next(){
        int index = this.businessCards.indexOf(this.current);
        index++;
        if(index >= this.businessCards.size() ){
            index = this.businessCards.size() - 1;
        }
        
        this.current = this.businessCards.get(index);
        return this.current;
    }
    public BusinessCard Last(){
        this.current = this.businessCards.getLast();
        
        return this.current;
    }
    public BusinessCard Move(BusinessCard index){
        int i = this.businessCards.indexOf(index);
        this.current = this.businessCards.get(i);
        return this.current;
    }
    public int GetLength(){
        return this.length;
    }
    public BusinessCard GetCurrent(){
        return this.current;
    }
    
    private LinkedList<BusinessCard> businessCards;
    private int length;
    private BusinessCard current;
    /*
    public static void main(String[] args) {
        BusinessCard businessCard1 = new BusinessCard("홍컴", "서울시 마포구", "02111", "021112", "hong.com","홍길동", " 01011111", "hong@", "사장");
	BusinessCard businessCard2 = new BusinessCard("고컴", "대전시 유성구", "041111", "041112", "ko.com","고길동", "0102222", "ko@", "과장");
	BusinessCard businessCard3 = new BusinessCard("이컴", "대구시 달서구", "05223333", "0522333", "Lee.com","홍길동", "01022222", "hong2@", "사원");
	BusinessCard businessCard4 = new BusinessCard("홍컴", "서울시 마포구", "021112", "0211123", "hong.com",	"정길동", "010333333", "jung@", "대리");
        BusinessCardHolder businessCardHolder = new BusinessCardHolder();
        BusinessCard index;
        index = businessCardHolder.PutIn(businessCard1);
        System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
        
        index = businessCardHolder.PutIn(businessCard2);
        System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
        
        index = businessCardHolder.PutIn(businessCard3);
        System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
        
        index = businessCardHolder.PutIn(businessCard4);
        System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
      
        index=businessCardHolder.First();
         System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         
         index=businessCardHolder.Next();
         System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         
         BusinessCard businessCard =businessCardHolder.PutOut(index);
         System.out.println(businessCard.GetCompanyName()+" "+businessCard.GetAddress()+" "+businessCard.GetCompanyTelephoneNumber()+
                " "+businessCard.GetFaxNumber()+" "+businessCard.GetUrl()+" "+ businessCard.GetPersonalName() + " " + businessCard.GetPersonalTelephoneNumber() +
                " " + businessCard.GetEmailAddress() + " " + businessCard.GetPosition() );
         index = businessCardHolder.PutIn(businessCard);
         System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         
         BusinessCard[] indexes;
         indexes = businessCardHolder.Find("홍길동");
         int i =0;
         while(i<indexes.length){
             System.out.println(indexes[i].GetCompanyName()+" "+indexes[i].GetAddress()+" "+indexes[i].GetCompanyTelephoneNumber()+
                " "+indexes[i].GetFaxNumber()+" "+indexes[i].GetUrl()+" "+ indexes[i].GetPersonalName() + " " + indexes[i].GetPersonalTelephoneNumber() +
                " " + indexes[i].GetEmailAddress() + " " + indexes[i].GetPosition() );
             i++;
         }
         
         index=businessCardHolder.Last();
         System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         index=businessCardHolder.Previous();
         System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         index=businessCardHolder.FindByCompanyName("홍컴");
         System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         
         index=businessCardHolder.Move(indexes[0]);
          System.out.println(index.GetCompanyName()+" "+index.GetAddress()+" "+index.GetCompanyTelephoneNumber()+
                " "+index.GetFaxNumber()+" "+index.GetUrl()+" "+ index.GetPersonalName() + " " + index.GetPersonalTelephoneNumber() +
                " " + index.GetEmailAddress() + " " + index.GetPosition() );
         
    }
    */
    
    
    
}
