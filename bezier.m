function bezier( vertices )
%BEZIER ����Bezier����
 

Dim=size(vertices,1);%��λ������ά�ռ�
NumPoint=size(vertices,2)-1;%��ĸ���
t=0:0.001:1;
x=[];y=[];z=[];
if Dim==2
     x=(1-t).^(NumPoint)*vertices(1,1);
     y=(1-t).^(NumPoint)*vertices(2,1);
  for j=1:NumPoint
      w=factorial(NumPoint)/(factorial(j)*factorial(NumPoint-j))*(1-t).^(NumPoint-j).*t.^(j);
      x=x+w*vertices(1,j+1);y=y+w*vertices(2,j+1);
  end
plot(vertices(1,:),vertices(2,:),'b');
hold on;grid on;
axis tight;  
xlabel('X');ylabel('Y');
plot(x,y,'r');
end

vertices=[0, 0;1000 ,50;0,100]'
bezier( vertices )
hold on;
vertices=[0, 0;0,100]';
bezier( vertices )