%%%%%%%%%%%%%%%%%%%%%%%%
% �й� :2013722095    %
% �а� : ��ǻ�� ���а� %
% �̸� : ������        %
%%%%%%%%%%%%%%%%%%%%%%%%%
Count_of_Eigenvector = 4;

Mean_Face = [];
set_mean = zeros(112*92, 1);
%%%%%%%%%%%%%%%%%%% Calculate mean face of each Data set%%%%%%%%%%%%%%%%%%%
for i = 1 : 1 : 6           % folder
    dir_name = strcat('PCA_DB\Training\set_', int2str(i), '\');
    mean = zeros(112*92, 1);
    for j = 1 : 1 : 8       % bmp file
        file_name =  strcat(dir_name, int2str(j), '.bmp');
        disp(file_name);
        data = imread(file_name);
        data = rgb2gray(data);
        data = reshape(data, 112*92, 1);        % 112*92�� X 1���� ���·� ����
        mean = mean + double(data);
    end
    mean = mean/8;          %mean of set_'i'
    Mean_Face = [Mean_Face mean];
    set_mean = set_mean + mean;
end

set_mean = set_mean/6;  %mean of all face

normalized_vector = [];
for j = 1 : 1 : 6
    vector = double(Mean_Face(:, j)) - set_mean;
    normalized_vector = [normalized_vector vector];
end

message = strcat('Normalized�� �Ϸ�Ǿ����ϴ�.');
disp(message);

Covariance_matrix = normalized_vector * transpose(normalized_vector);

message = strcat('Covariance_matrix ������ �Ϸ�Ǿ����ϴ�.');
disp(message);

[Eigenvector, Eigenvalue] = eig(Covariance_matrix);

message = strcat('Eigenvector, Eigenvalue ������ �Ϸ�Ǿ����ϴ�.');
disp(message);
   
%������ ���Ͱ� �����̰� ���Ͱ� �� ������ ��Ÿ���Ƿ� �������� ���� ���� 10�� ���� �ɵ�
% eig�Լ��� ������ ���͵� �ְ� ����� ��
% ���Ͷ� ����� ���δ� ������ -> ū�� ������ ���ĵǾ�����.
%%%%% ���� 10���� ���� eigenvector ����
% �ִ� �Ŵ� �����ڷ��� ū�ͺ��� ����
real_Eigenvector  = [];
for j = 10304: -1: (10304 - Count_of_Eigenvector + 1)
    real_Eigenvector = [real_Eigenvector Eigenvector(:,j)];
end

%%%%%%%%%%%%% input�� ���� ~x1, ~x2 ....���� %%%%%%%%%%%%%5
Classifier = [];
for j = 1 : 1 : 6
     feature = transpose(real_Eigenvector) * normalized_vector(:, j);    % get traindata's feature
     Classifier = [Classifier feature];
end  

message = strcat('��ü face�鿡 ���� Classifier ������ �Ϸ�Ǿ����ϴ�.');
disp(message);

%%%%%%%%%%%% test set recognition %%%%%%%%%%%%%%%%%%%%%%%
dir_name = strcat('PCA_DB\', 'Test\');
for i = 1 : 1 : 12           % test file
    file_name =  strcat(dir_name, int2str(i), '.bmp');
    data = imread(file_name);
    data = rgb2gray(data);
    data = reshape(data, 112*92, 1);        % 1(��) * (112*92)(��)�� ���·� ����
    normalized_vector = double(data) - set_mean;
    feature = transpose(real_Eigenvector) * normalized_vector;
    
     %%%%%%% ��Ŭ���� �Ÿ��� %%%%%%%
    distance_set = [];
    for j = 1 : 1 : 6  % 6���� class�� ���ؼ� 
        distance = 0;
        for k = 1: 1: Count_of_Eigenvector   % �� component �� ���̰��� ���� �ջ�
            distance = distance + (Classifier(k,j) - feature(k,1))^2;
        end
        distance = sqrt(distance); % ������ 
        distance_set = [distance_set distance];
    end
    
    % �ּҰ� Ž�� : min()���� �Ϸ������� �Ǽ��� ���� ������ �����̾���Ѵ� �ؼ� ���� ����
    least = 1;
    for j = 2: 1: 6
        if(distance_set(1, least) > distance_set(1, j))
            least = j;
        end
    end
        
    result = strcat('Recognition of test file[ ',num2str(i),'.bmp] : ', num2str(least));
    disp(result);
end
